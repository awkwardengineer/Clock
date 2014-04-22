>**Executive Summary:** Offload as much work to the TIMERs as possible and shutting down the CPU to save power.

##Software/Power Strategy
To truly take advantage of what the ATtiny84 microcontroller can do, we're going to be getting into some of the features of the chip that the Arduino environment normally hides. More specifically, we'll be taking advantage of some of the power saving modes.

As it turns out, the ATtiny84 has two timers that can be run completely independently of the actual CPU (plus a watchdog, which we won't use). The general idea, therefore, is to keep the CPU off as much as possible, saving power, and only doing work during periodic interrupt service routines when TIMER1 maxes out.

The first timer, TIMER0, will be used to control the PWM outputs that control the hands on the voltmeter. When initialized with the right settings, the timer will chug along, independently of the CPU and the Output Compare Registers A & B will automatically compare their settings to the timer count, setting the duty cycle of the PWM outputs.

The second timer, TIMER1, has a 16 bit register that triggers an interrupt when it overflows, waking up the CPU. The microcontroller then polls the input to see if the "Mode select" knob has changed, adjusts the hands as appropriate, then goes back to sleep.

We started making some notes 