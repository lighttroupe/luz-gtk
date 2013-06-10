#include "utils.h"

#include "message-bus.h"

#include "Leap.h"
//#include "LeapUtilGL.h"

#define ADDRESS_BUFFER_SIZE (1000)

#define MAX_DEPTH (10000)

#define MAX_USERS_TRACKED (20)

//
// NOTE: initializing to 0 is proper for all fields
//
typedef struct {
	bool tracked;
	uint human_number;

	TLimits limits_position_x;
	TLimits limits_position_y;
	TLimits limits_position_z;

	TLimits limits_roll;
	TLimits limits_pitch;
	TLimits limits_yaw;

	TLimits limits_sphere_radius;
} THuman;

class HandTracker : public Leap::Listener
{
public:
	HandTracker();
	virtual ~HandTracker();

	static Leap::Controller& get_leap_controller();
	virtual void onInit(const Leap::Controller&);
	virtual void onConnect(const Leap::Controller&);
	virtual void onDisconnect(const Leap::Controller&);
	virtual void onFocusGained(const Leap::Controller&);
	virtual void onFocusLost(const Leap::Controller&);
	virtual void onFrame(const Leap::Controller& controller);

	void update();
	void draw();

	uint get_max_humans() { return m_max_humans; }
	void set_max_humans(uint max_humans) { m_max_humans = (max_humans <= MAX_USERS_TRACKED) ? max_humans : MAX_USERS_TRACKED; }

	uint get_human_number_offset() { return m_human_number_offset; }
	void set_human_number_offset(uint human_number_offset) { m_human_number_offset = human_number_offset; }

	bool is_leap_initialized() { return m_leap_initialized; }

private:
	bool init_leap();

	uint m_max_humans;
	THuman m_humans[MAX_USERS_TRACKED];
	uint m_human_number_offset;

	uint m_hands_present;

	bool m_leap_initialized;
};
