#An Intro To Microcontroller Selection From Someone Who's Only Selected One or Two Before

>**Executive Summary: I selected an ATtiny84 microchip. It's the next bump up in terms of I/O from Atmel. The decision was based on experience with the prototype clock, which was built on a Digispark Board/ATtiny85, and didn't have enough I/O**

Selecting a microcontroller can be overwhelming. The general idea is to pick something powerful enough (but not too powerful, and therefore wasteful!) for your application. Sounds good in theory, but when each major chipmaker has literally HUNDREDS of chips to do from, where do you start?

The process we followed used a mixture of general knowledge that had come from experience hanging out with electrical engineers (@AwkwardEngineer has a college degree that says mechanical engineer), blind luck, and some prototyping.

##The Process

1.  Prototype with a Digispark board based around the 6 I/O pin ATtiny85
2.  Clock functioned but was missing features
3.  Estimate program size based on prototype code, and count required pins.
4.  Decide to stay with Atmel chips (mostly by default)
5.  Use Atmel's selection tool to pick chip based on I/O, memory, and cost.


As a starting point, the prototype uses a Digispark, which is built around an Atmel ATtiny85 chip, which has 6 free pins available, and even gets double duty out of the pins, using a  burnt-in program called a bootloader to switch some of the pins from programming inputs to whatever the end user wants to use them for.

#Selection Decision 1: Stay with Atmel
Unfortunately, the 6 pins aren't enough if I want to add a proper oscillating crystal so the clock will stay accurate. (The ATtiny85 has an internal oscillator, but it drifts noticeably every hour.)  I do however want to stay with Atmel, because the chips are cheap, readily available in small batch volumes, and the chips are well supported through the Arduino community.

### The pin requirements:
Below is a block diagram of the clock circuit, which is followed by the pin requirements.

![block diagram](http://3.bp.blogspot.com/-WZm3e-HqyCU/Uy90zO9LAzI/AAAAAAAAT7A/TaeWjGZixP4/s1600/block+diagram.png)

**Pin summary**  
1 analog in to read selector switch / voltage divider  
2 interrupts to read quadrature encoder  
2 PWM outputs  
3 digital outputs for am/pm light, alarm set, and buzzer  
2 pins for external oscillator crystal  


All together, this works out to 8 I/O pins, the 2 clock pins, and whatever is needed for In System Programming (ISP) pins, which can serve double duty. (If anybody would like to explain a general lesson for doing ISP without interfering with the existing peripheral circuits, I would love to get one.)

Atmel provides a somewhat overwhelming product selector, and if you set the number of input pins higher than 6, you see that next category of chip has at least 11 inputs.  I also know from my prototype that the compiled code was 3kb in size, plus 2kb for the bootloader, so I need a chip with >5k memory.

Using the selector, that narrows down to the **ATtiny84**, with

8kB memory [check!]  
Interrupts galore [check!]  
2 PWM channels [check!]  
Digital channels galore [check!]  
External clock pins [check!]  

Plus, it has support for ISP programming.
##To expand on for next time:
Details of the chip pin outs, connecting the oscillator, ISP programming, and more.
