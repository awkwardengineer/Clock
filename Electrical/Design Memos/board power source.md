#Selecting a Power Source

We've gotten some feedback to ditch the wall-wart power supply and move the clock over to battery power. That certainly has some appeal (Ditches a pricey component, eliminates the assembly operation to feed the cable through the wall, eliminates a grommet at the wall, etc...), so we decided to look into it.

#Is battery power reasonable?

Wikipedia gives the capacity of an [AA battery](http://en.wikipedia.org/wiki/AA_battery) as 1800-2600mAHr. The ATtiny84 draws 300uA and the two ammeters draw an average total of 50uA. With some assumptions about an AA battery and some simple math:

2200mAHr / 350uA = 6286 Hrs = 261 days = about 8 months.

8 months is pretty good! And that's without taking advantage of the microcontroller sleep modes. I'd feel pretty good about performance if I could stretch it to a full year. I think adding a battery in parallel would provide good margin to do that.

#Implications of Switching to Battery Power
Other than the need to conserve power, the next challege is that the battery voltage will droop as it discharges. The ATtiny84 has some brown out detection stuff I don't know much about, but more importantly, drooping voltage can't effect the position of the time keeping analog needles.

There are a few ways I'm considering to deal with this problem:

|                                                                                                                   | Pros                         | Cons                         |   |   |
|-------------------------------------------------------------------------------------------------------------------|------------------------------|------------------------------|---|---|
| Linear regulator on battery                                                                                       | simple                       | less efficient               |   |   |
| Buck (and/or) Boost converter on battery                                                                          | more expensive               | more efficient               |   |   |
| Leave the battery unregulated, but use a voltage reference to make a constant current source to drive the meters. | unsure of cost or efficiency | unsure of cost or efficiency |   |   |