#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

/**
  * @brief Functionality to store the current FSM state and keep track of the time within said state
  */

template<typename T>
class StateManager {
    /**
      * @brief StateManager (class) - class acting as FSM and managing states along with timing information
      * @param T - enum containing states
      */
    public:
        using State_t = T;

    private:
        State_t _state;

        unsigned long _time;

    public:
        /**
          * @brief StateManager (constructor) - initialises FSM to initial state
          * @param const State_t - initial state from enum provided to begin FSM at
          */
        inline StateManager(const State_t initial) noexcept
        {
            this->_state = initial;
            this->_time = millis();
        }

        /**
          * @brief state (overload) - sets FSM to provided state
          * @param const State_t - state from enumto set FSM to
          */
        inline void state(const State_t state, const bool force=false) noexcept
        {
            if( state != this->_state || force )
            {
                this->_state = state;
                this->_time = millis();
            }
        }

        /**
          * @brief state (overload) - gets FSM state
          * @return State_t - state from enum FSM is set to
          */
        inline State_t state() const noexcept
        {
            return this->_state;
        }

        /**
          * @brief since - gets time FSM entered a given state
          * @return ros::Time - time FSM entered a given state
          */
        inline const unsigned long since() const noexcept
        {
            return this->_time;
        }

        /**
          * @brief duration - gets duration FSM has been in a given state
          * @return ros::Duration - time FSM has been in its given state
          */
        inline const unsigned long duration() const noexcept
        {
            return millis() - this->since() ;
        }

        /**
          * @brief reset_time - resets time FSM entered a given state
          */
        inline void reset_time() noexcept
        {
            this->_time = millis();
        }
} ;

#endif // STATE_MANAGER_H
