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
	return false;
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
	printf("leap:onInit\n");
}

void HandTracker::onConnect(const Leap::Controller&)
{
	printf("leap:onConnect\n");
}

void HandTracker::onDisconnect(const Leap::Controller&)
{
	printf("leap:onDisconnect\n");
}

void HandTracker::onFocusGained(const Leap::Controller&)
{
}

void HandTracker::onFocusLost(const Leap::Controller&)
{
}

void HandTracker::onFrame(const Leap::Controller& controller)
{
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

	glClearColor(UNINITIALIZED_BACKGROUND_COLOR);
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
