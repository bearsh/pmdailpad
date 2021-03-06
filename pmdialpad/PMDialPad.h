/*
 * Copyright (c) 2016, Martin Gysel, me@bearsh.org
 * SPDX-License-Identifier: GPL-3.0+
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PMDIALPAD_H_
#define PMDIALPAD_H_

#include "mbed-drivers/mbed.h"
#include "bearsh-drivers/AnalogIn.h"
#include "bearsh-drivers/InterruptInOut.h"

#define SIMPLEKEYBOARD_NB_CONVERT_RUN   3

class PMDialPad {
public:
	enum Key {
		KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_S, KEY_R,
		KEY_INVALID, KEY_NONE, KEY_LIST_LEN
	};

	PMDialPad(PinName button, PinName scale);
	virtual ~PMDialPad();

	enum Key button;
	int holdTime;

	void attachOnPressedCallback(void (*cb)(enum Key)) {
		pressed_cb.attach(cb);
	}
	template<typename T>
	void attachOnPressedCallback(T *object, void (T::*member)(enum Key)) {
		pressed_cb.attach(object, member);
	}

	void attachOnReleasedCallback(void (*cb)(enum Key)) {
		released_cb.attach(cb);
	}
	template<typename T>
	void attachOnReleasedCallback(T *object, void (T::*member)(enum Key)) {
		released_cb.attach(object, member);
	}

	void attachOnHoldCallback(void (*cb)(enum Key)) {
		hold_cb.attach(cb);
	}
	template<typename T>
		void attachOnHoldCallback(T *object, void (T::*member)(enum Key)) {
		hold_cb.attach(object, member);
	}

private:
	bearsh::AnalogIn anaIn;
	bearsh::InterruptInOut intIn;
	DigitalInOut scaleOut;
	enum {
		IDLE, INTERRUPT, CONVERT, DEBOUNCE, PRESSED, RELEASED, MEASUREMENT
	} status;
	int convert_run;
	int8_t key_values[SIMPLEKEYBOARD_NB_CONVERT_RUN];
	uint8_t keys[KEY_LIST_LEN];
	minar::callback_t timeout_evt;
	minar::callback_t adc_evt;
	minar::callback_t holdtime_evt;
	minar::callback_handle_t holdtime_evt_handle;
	mbed::util::FunctionPointer1<void, enum Key> pressed_cb;
	mbed::util::FunctionPointer1<void, enum Key> released_cb;
	mbed::util::FunctionPointer1<void, enum Key> hold_cb;

	void timeout();
	void buttonPress();
	void adcInterrupt();
	void adcDone();
	void measureMode();
	void interruptMode();
	void holdTimeout();
};

#endif /* PMDIALPAD_H_ */
