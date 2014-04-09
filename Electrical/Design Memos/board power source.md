#Selecting a Power Source

We've gotten some feedback to ditch the wall-wart power supply and move the clock over to battery power. That certainly has some appeal (Ditches a pricey component, eliminates the assembly operation to feed the cable through the wall, eliminates a grommet at the wall, etc...), so we decided to look into it.

#Is battery power reasonable?

Wikipedia gives the capacity of an [AA battery](http://en.wikipedia.org/wiki/AA_battery) as 1800-2600mAHr. The ATtiny84 draws 300uA and the two ammeters draw an average total of 50uA. With some assumptions about an AA battery and some simple math:

2200mAHr / 350uA = 6286 Hrs = 261 days = about 8 months.

8 months is pretty good! And that's without taking advantage of the microcontroller sleep modes. I'd feel pretty good about performance if I could stretch it to a full year. I think adding a battery in parallel would provide good margin to do that.

#Implications of Switching to Battery Power
Other than the need to conserve power, the next challenge is that the battery voltage will droop as it discharges. The ATtiny84 has some brown out detection stuff I don't know much about, but more importantly, drooping voltage can't effect the position of the time keeping analog needles.

There are a few ways I'm considering to deal with this problem:

|Method                                                                                                             | Pros                         | Cons                         |
|-------------------------------------------------------------------------------------------------------------------|------------------------------|------------------------------|
| Linear regulator on battery                                                                                       | simple                       | less efficient               |
| Buck (and/or) Boost converter on battery                                                                          | more expensive               | more efficient               |
| Leave the battery unregulated, but use a voltage reference to make a constant current source to drive the meters. | unsure of cost or efficiency | unsure of cost or efficiency |


The ATtiny84 can run on anything from 1.8V to 5V, so it means 2 or 3 AA batteries in series off the bat. According to [this source](http://www.powerstream.com/AA-tests.htm), an AA alkaline battery will slowly droop from 1.5V to 1V over the course of it's life before really crashing.



##Linear Regulator
Working again through the firehose that is Digikey, linear regulator chips like [this](http://www.ti.com/lit/ds/symlink/lp2951-33.pdf) are available for about $.20-.40 in the 100 piece volume range. This is pretty cheap, but the trade is that the two AA batteries will have an average voltage of about 2.5V over their lifespan and if I'm trying to output 2V or so to the micro, bucking that .5V difference is like a 20% hit to power. Instead of the board pulling 350uA, it'd seem more like 420uA.

##Buck Boost



