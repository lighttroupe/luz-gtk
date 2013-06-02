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
	void send();

	uint get_max_humans() { return m_max_humans; }
	void set_max_humans(uint max_humans) { m_max_humans = (max_humans <= MAX_USERS_TRACKED) ? max_humans : MAX_USERS_TRACKED; }

	uint get_human_number_offset() { return m_human_number_offset; }
	void set_human_number_offset(uint human_number_offset) { m_human_number_offset = human_number_offset; }

	bool is_leap_initialized() { return m_leap_initialized; }

private:
	//
	// Human Number tracking
	//
	uint next_human_number();
	void set_human_number_for_user_id(int id, uint human_number);
	uint user_id_to_human_number(int user_id);
	int human_number_to_user_id(uint human_number);
	THuman* user_id_to_human(int user_id);
	void reassign_human_number(uint human_number);

	TLimits3 m_limits_stage;

	//
	// OpenNI
	//
	bool init_leap();

	uint m_max_humans;
	THuman m_humans[MAX_USERS_TRACKED];
	uint m_human_number_offset;

	bool m_leap_initialized;
	bool m_draw_depth_map;
};
