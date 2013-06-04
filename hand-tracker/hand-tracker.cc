#include "application.h"
#include "hand-tracker.h"

#include <GL/gl.h>

#define STARTING_JOINT_BOUNDING_BOX_WIDTH (400.0)
#define DEFAULT_MAX_HUMANS (1)

#define BACKGROUND_COLOR 0.0, 0.0, 0.0, 0.0
#define UNINITIALIZED_BACKGROUND_COLOR 0.6, 0.1, 0.1, 0.0

#define NUM_HUMAN_COLORS (9)
static float HUMAN_COLORS[NUM_HUMAN_COLORS][4] = {
	{0.800, 0.800, 0.800, 1.0},	// no human number assigned

	{1.000, 0.000, 0.000, 0.5},
	{0.000, 1.000, 0.000, 0.5},
	{0.000, 0.000, 1.000, 0.5},
	{1.000, 1.000, 0.000, 0.5},

	{0.500, 0.000, 0.000, 1.0},
	{0.000, 0.500, 0.000, 1.0},
	{0.000, 0.000, 0.500, 1.0},
	{0.500, 0.500, 0.000, 1.0},
};

#define DEG_TO_RAD (0.0174532925f)
#define RAD_TO_DEG (57.295779513f)

//
// HandTracker
//
HandTracker::HandTracker()
	: m_max_humans(DEFAULT_MAX_HUMANS),
		m_human_number_offset(0)
{
	memset(m_humans, 0, sizeof(m_humans));

	m_leap_initialized = init_leap();
}

bool HandTracker::init_leap()
{
	get_leap_controller().addListener( *this );
	return true;
}

//
// UserID => Human mapping
//
THuman* HandTracker::user_id_to_human(int user_id)
{
	return &m_humans[user_id-1];
}

uint HandTracker::user_id_to_human_number(int user_id)
{
	THuman* human = user_id_to_human(user_id);
	return(human ? human->human_number : 0);
}

int HandTracker::human_number_to_user_id(uint human_number)
{
	for(int id=1 ; id <= MAX_USERS_TRACKED ; id++) {
		if(user_id_to_human_number(id) == human_number)
			return id;
	}
	return 0;
}

Leap::Controller& HandTracker::get_leap_controller()
{
		static Leap::Controller s_controller;
		return  s_controller;
}

void HandTracker::onInit(const Leap::Controller&)
{
	printf("leap:initialized\n");
}

void HandTracker::onConnect(const Leap::Controller& controller)
{
	printf("leap:connected\n");

/*
	controller.enableGesture(Leap::Gesture::TYPE_CIRCLE);
	controller.enableGesture(Leap::Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Leap::Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Leap::Gesture::TYPE_SWIPE);
*/
}

void HandTracker::onDisconnect(const Leap::Controller&)
{
	printf("leap:disconnected\n");
}


void HandTracker::onFocusGained(const Leap::Controller&)
{
	printf("leap:focused\n");
}

void HandTracker::onFocusLost(const Leap::Controller&)
{
	printf("leap:unfocused\n");
}

void HandTracker::onFrame(const Leap::Controller& controller)
{
	Leap::Frame frame = controller.frame();

	char address_buffer[ADDRESS_BUFFER_SIZE+1];

//	std::cout << "Frame id: " << frame.id()
//						<< ", timestamp: " << frame.timestamp()
//						<< ", hands: " << frame.hands().count()
//						<< ", fingers: " << frame.fingers().count()
//						<< ", tools: " << frame.tools().count() << std::endl;

	if (!frame.hands().empty()) {
		// Get the first hand
		const Leap::Hand hand = frame.hands()[0];
		THuman* human = &m_humans[0];
		int human_number = 1;
		float fuzzy;

		fuzzy = scale_and_expand_limits(hand.palmPosition().x, &human->limits_position_x);
		snprintf(address_buffer, ADDRESS_BUFFER_SIZE, "Hand %02d / Position / X", human_number);
		g_message_bus->send_float(address_buffer, fuzzy);

		fuzzy = scale_and_expand_limits(hand.palmPosition().y, &human->limits_position_y);
		snprintf(address_buffer, ADDRESS_BUFFER_SIZE, "Hand %02d / Position / Y", human_number);
		g_message_bus->send_float(address_buffer, fuzzy);

		fuzzy = scale_and_expand_limits(hand.palmPosition().z, &human->limits_position_z);
		snprintf(address_buffer, ADDRESS_BUFFER_SIZE, "Hand %02d / Position / Z", human_number);
		g_message_bus->send_float(address_buffer, fuzzy);

		// Get the hand's normal vector and direction
		const Leap::Vector normal = hand.palmNormal();
		const Leap::Vector direction = hand.direction();

		// Calculate the hand's pitch, roll, and yaw angles
		//std::cout << "hand pitch: " << direction.pitch() << " rad, " << std::endl
							//<< "hand  roll: " << normal.roll() << " rad, " << std::endl
							//<< "hand   yaw: " << direction.yaw() << " rad" << std::endl << std::endl;

		fuzzy = scale_and_expand_limits_with_clamp(direction.pitch(), -0.9, 0.9, &human->limits_pitch);
		snprintf(address_buffer, ADDRESS_BUFFER_SIZE, "Hand %02d / Pitch", human_number);
		g_message_bus->send_float(address_buffer, fuzzy);

		fuzzy = scale_and_expand_limits_with_clamp(normal.roll(), -1.0, 1.0, &human->limits_roll);
		snprintf(address_buffer, ADDRESS_BUFFER_SIZE, "Hand %02d / Roll", human_number);
		g_message_bus->send_float(address_buffer, fuzzy);

		fuzzy = scale_and_expand_limits_with_clamp(direction.yaw(), -0.8, 0.8, &human->limits_yaw);
		snprintf(address_buffer, ADDRESS_BUFFER_SIZE, "Hand %02d / Yaw", human_number);
		g_message_bus->send_float(address_buffer, fuzzy);

		fuzzy = scale_and_expand_limits_with_clamp(hand.sphereRadius(), 75.0, 100.0, &human->limits_sphere_radius);
		snprintf(address_buffer, ADDRESS_BUFFER_SIZE, "Hand %02d / Sphere Radius", human_number);
		g_message_bus->send_float(address_buffer, fuzzy);

		// TODO: first finger is pointer
	}

/*
		// Get gestures
	const Leap::GestureList gestures = frame.gestures();
	for (int g = 0; g < gestures.count(); ++g) {
		Leap::Gesture gesture = gestures[g];

		switch (gesture.type()) {
			case Leap::Gesture::TYPE_CIRCLE:
			{
				Leap::CircleGesture circle = gesture;
				std::string clockwiseness;

				if (circle.pointable().direction().angleTo(circle.normal()) <= PI/4) {
					clockwiseness = "clockwise";
				} else {
					clockwiseness = "counterclockwise";
				}

				// Calculate angle swept since last frame
				float sweptAngle = 0;
				if (circle.state() != Leap::Gesture::STATE_START) {
					Leap::CircleGesture previousUpdate = Leap::CircleGesture(controller.frame(1).gesture(circle.id()));
					sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
				}
				std::cout << "Circle id: " << gesture.id()
									<< ", state: " << gesture.state()
									<< ", progress: " << circle.progress()
									<< ", radius: " << circle.radius()
									<< ", angle " << sweptAngle * RAD_TO_DEG
									<<  ", " << clockwiseness << std::endl;
				break;
			}
			case Leap::Gesture::TYPE_SWIPE:
			{
				Leap::SwipeGesture swipe = gesture;
				std::cout << "Swipe id: " << gesture.id()
									<< ", state: " << gesture.state()
									<< ", direction: " << swipe.direction()
									<< ", speed: " << swipe.speed() << std::endl;
				break;
			}
			case Leap::Gesture::TYPE_KEY_TAP:
			{
				Leap::KeyTapGesture tap = gesture;
				std::cout << "Key Tap id: " << gesture.id()
									<< ", state: " << gesture.state()
									<< ", position: " << tap.position()
									<< ", direction: " << tap.direction()<< std::endl;
				break;
			}
			case Leap::Gesture::TYPE_SCREEN_TAP:
			{
				Leap::ScreenTapGesture screentap = gesture;
				std::cout << "Screen Tap id: " << gesture.id()
									<< ", state: " << gesture.state()
									<< ", position: " << screentap.position()
									<< ", direction: " << screentap.direction()<< std::endl;
				break;
			}
			default:
				std::cout << "Unknown gesture type." << std::endl;
				break;
		}
	}
*/
}

void HandTracker::set_human_number_for_user_id(int id, uint human_number)
{
	THuman* human = user_id_to_human(id);
	if(human->human_number != 0) {		// TODO: and number of active humans is < max number
		// reassign_human_number(human->human_number);
	}
	memset(human, 0, sizeof(THuman));
	human->human_number = human_number;
}

uint HandTracker::next_human_number()
{
	// Look for free numbers, favoring low numbers
	for(int number=1 ; number <= get_max_humans() ; number++) {
		if(human_number_to_user_id(number) == 0) {
			return number;
		}
	}
	return 0;
}

//
// Update
//
void HandTracker::update()
{
}

//
// Draw
//
void HandTracker::draw()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(is_leap_initialized()) {
		glClearColor(BACKGROUND_COLOR);
	} else {
		glClearColor(UNINITIALIZED_BACKGROUND_COLOR);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_TEXTURE_2D);
}

//
// Send OSC
//
void HandTracker::send()
{
	char address_buffer[ADDRESS_BUFFER_SIZE+1];
	int human_number = 1;

	// HACK: Send Tracked = 1
	snprintf(address_buffer, ADDRESS_BUFFER_SIZE, "Hand %02d / Tracked", human_number);
	g_message_bus->send_int(address_buffer, 1);
}

HandTracker::~HandTracker()
{
}
