>**Executive Summary:** Browsing digikey found no appropriate SMT encoders or pots. Of the through-hole connectors, the "right angle" style are $.10-.20 more expensive, but I believe that is justified by the fact a single board can connect the knobs and dials, reducing assembly complexity, and eliminating the need for flying leads or jumpers to the dials. Quantities may necessitate a special order through a distributor. [Bourns](http://www.bourns.com/Products.aspx) was the company picked to select encoders and pots from. I picked the Bourns **PEC12R-2020F-N0024** encoder, and the Bourns **PTV11-2420A-A4503** potentiometer.

##Intro
The Clock has two control knobs, one to select the mode of operation, and one to make adjustments to time or calibration, depending on the mode. The mode select knob uses a pot to measure absolute position and the adjustment knob uses a quadrature encoder to allow the knob to rotate 360 degrees and to measure relative motion.

![knobs](/Photos/Blog Post Material/controlknobs.png)

##The Prototype, aka The Rat's Nest
The prototype has a lot of hand soldered flying leads, cobbled together from knob components that I found, the Digispark microcontroller, and the voltmeters.

![rat nest](/Photos/Blog Post Material/screwterminal.png)

##Designing for Assembly
The electronics assembly absolutely needs to be cleaned up so that assembly process can be quick and reliable. I've worked in the past with a mechanical assembly house that provide employment for the mentally disabled and I try to keep them in mind when designing. The assembly house is also near my e-commerce warehouse, so I have to have a really good reason to not use them. Poor design can't be one of those reasons.

I had two ideas:

1. Use panel mount switches to hold the PCBA underneath the clock. The flying leads running to the dials would be replaced with a jumper kit, with a plug on one end of the harness running to the board and ring terminal crimps on the other that would screw onto the dial leads. I figured the overall board size would be smaller (and therefore cheaper), and the cable, if I could source it, would be easy for an unskilled laborer to install. The nuts on "panel mount" style knobs would secure the PCBA and eliminate the need for standoffs in the housing.<br>![jumper](/Photos/Blog Post Material/jumper.png)![panel mount](/Photos/Blog Post Material/panelmount.png)
2. I had a second idea to use the board itself to run traces to a through hole. The area around the hole would be plated so that screwing the board to the dial leads would make electrical contact and mechanically mount the board. This would require a "right angle" style encoder and pot. <br>![single](/Photos/Blog Post Material/singleboard_CAD.png)

##Searching Digikey
Much like microcontroller selection, browsing the zillions of components on Digikey can be overwhelming. Here's what drove selection:

- sorting by price, I found 6mm shafts were typically cheapest. (closest metric size to 1/4in and a common knob size. I learned this from work on the Awkard Engineer [Panic Button Light Switch Kit](http://www.awkwardengineer.com/collections/stuff/products/panic-button-limited-edition))
- SMTs were out pretty quickly, they're just not available in the right sizes or form factors.
- next I learned that right angle style components were running around $.10-.20 more expensive than straight on style devices, but given that it saves sourcing a jumper cable and two nut turning operations on the panel mount style switch, I think it's worth it.
- finally, try to select by size / price.

Of the Digkey components I found, [Bourns](http://www.bourns.com/Products.aspx) was a name that popped up for in stock products in the sub $1 range for components.

##A comment on sourcing before final selection
I know I'm too small a player to deal with a manufacturer directly and my volumes may necessitate a special order, given what I saw on Digikey. Rather than designing based on what's in-stock with Digikey right *now*, my approach will be to take the manufacturer I identified, select an encoder and pot with identical packaging (will make mechanical assembly easier) and pick what I really want.

An EE friend suggested that when you cross over from the on-demand, prototype quantities available through Digi-key, there's another market for chips out there with a mix of distributors and "gray market" sources. Manufacturers take multi thousand piece orders (think whole reels, trays, or cases... in retail we'd call this the Minimum Order Quantity, or MOQ)from middlemen, then little guys like me buy from the middleman.

For example, a distributor may place the MOQ sized order and hold the balance in stock while selling the smaller quanitity to me, or a board house who had to place a MOQ sized order to populate a PCBA might try to sell the balance of their stock on the gray market.

The EE friend refered me specifically to [findchips.com](http://www.findchips.com/) as a "meta" search engine, that will search all the distributors for a paricular part and return the amount in stock. If the components aren't available there, then I'd place an order through a distributor and the lead time might be about 8-12 weeks.


##Selecting components
Finally, after picking Bourns's catalog, I used the parts I found using the "sort by price" option on Digikey to direct me to a specific "series" of part.

Within each series, you can pick from a range of options like mounting style, shaft length, encoder counts per revolution, press on / press off switch, and so on.  I picked

encoder: PEC12R-2020F-N0024
potentiometer: PTV11-2420A-A4503

When it comes time to go to fab, I may work with the board house to come up with a list of alternate components that would be acceptable. (For example, a 20K pot would be just as good as a 50K pot for my purposes).

##Physical Check
I modeled the components in CAD to physically check them. It looks like the single board idea will work, but I'll either need to stand it off from the screw terminals, or move the control knobs forward. (I know which way is cheaper, but also which way affects the look of the finished part)

![CAD Check](/Photos/Blog Post Material/component_check.png)

