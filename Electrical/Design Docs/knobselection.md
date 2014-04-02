>**Executive Summary:** Browsing digikey found no appropriate SMT encoders or pots. Of the through-hole connectors, the "right angle" style are $.10-.20 more expensive, but I believe that is justified by the fact a single board can connect the knobs and dials, reducing assembly complexity, and eliminating the need for flying leads or jumpers to the dials. [Bourns](http://www.bourns.com/Products.aspx) was the company picked to select encoders and pots from.

##Intro
The Clock has two control knobs, one to select the mode of operation, and one to make adjustments to time or calibration, depending on the mode. The mode select knob uses a pot to measure absolute position and the adjustment knob uses a quadrature encoder to allow the knob to rotate 360 degrees and to measure relative motion.

![knobs](/Photos/Blog Post Material/controlknobs.png)

##The Prototype, aka The Rat's Nest
The prototype has a lot of hand soldered flying leads, cobbled together from knob components that I found, the Digispark microcontroller, and the voltmeters.

![rat nest](/Photos/Blog Post Material/screwterminal.png)

##Designing for Assembly
The electronics assembly absolutely needs to be cleaned up so that assembly process can be quick and reliable.

I had two ideas:

1. Use panel mount switches to hold the PCBA underneath the clock. The flying leads running to the dials would be replaced with a jumper kit, with a plug on one end of the harness running to the board and ring terminal crimps on the other that would screw onto the dial leads. I figured the overall board size would be smaller (and therefore cheaper), and the cable, if I could source it, would be easy for an unskilled laborer to install. The nuts for the panel mount switch would secure the PCBA and eliminate the need for standoffs in the housing.<br>![jumper](/Photos/Blog Post Material/jumper.png)![panel mount](/Photos/Blog Post Material/panelmount.png)
2. I had a second idea to use the board itself to run traces to a through hole. The area around the hole would be plated so that screwing the board to the dial leads would make electrical contact and mechanically mount the board. This would require a "right angle" style encoder and pot. <br>![single](/Photos/Blog Post Material/singleboard.png)

##Searching Digikey
Much like microcontroller selection, browsing the zillions of components on Digikey can be overwhelming. Here's what drove selection:

- sorting by price, I found 6mm shafts were typically cheapest. (closest metric size to 1/4in and a common knob size. learned this from work on the Awkard Engineer [Panic Button Light Switch Kit](http://www.awkwardengineer.com/collections/stuff/products/panic-button-limited-edition))
- SMTs were out pretty quickly, they're just not available in the right sizes or form factors.
- next I learned that right angle style components were running around $.10-.20 more expensive than straight on style devices, but given that it saves sourcing a jumper cable and two nut turning operations on the panel mount style switch, I think it's worth it.
- finally, try to select by size / price.

##Selected components

Of the Digkey components I found, [Bourns](http://www.bourns.com/Products.aspx) was a name that popped up for in-stock products in the sub $1 range for components. They make both encoders and pots, which would be nice to get in similar packaging and have the right angle connectors fit with less hassle.
