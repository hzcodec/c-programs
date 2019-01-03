#ifndef FSM_H_
#define FSM_H_

// Stringify macros
#define XSTR(s) STR(s)
#define STR(s) #s

#define FSM(_name_, _type_, _init_) \
	static _type_ _name_##_state = (_init_); \
	_type_ _name_(void) { \
	static _type_ _prev_state = (_init_); \
	static _type_ _next_state = (_init_); \
	_type_ _state = _name_##_state; \
	switch (_state) {

//if (_next_state != _state) STATE_LOG(XSTR(_name_), _next_state, _state, _prev_state); \1
#define END_FSM(_name_) } \
	_prev_state = _state; \
	_state = _next_state; \
	_name_##_state = _state; \
	return _state; \
}
#define STATE(_state_) case (_state_): {
#define END_STATE } break;
#define ON_ENTRY if(_state != _prev_state)
#define NEXT_STATE(_state_) _next_state = (_state_)

#endif /* FSM_H_ */
