# BassMate

This is a simple drum machine and sequencer.

I want to be able to quickly create simple rhythm sequences to help me pratice electric bass, and was looking for something between a basic metronome and a full-fledged DAW.
The Drumbit online app (https://drumbit.app/) is pretty good, but having to point and click while playing an instrument is not tactile enough for me.
Accordingly, BassedMate (https://github.com/ukmaker/BassedMate) was born. This was the first iteration of the project, using a Teensy.

BassMate (this repo) is an evolution. I wanted to eliminate the VS1053 chip and I found the Electrosmith Daisy Seed (https://www.electro-smith.com/daisy/daisy)
which looked like it could do all the synthesis itself. I also found the MusicBoard project (https://github.com/julbouln/musicboard) which inspired me
to try to port to the Seed. This proved to be tricky, so I gave up (I'll try again one day!) and took a different path.

The UI of BassedMate felt a bit off, and when I came across the NavKey (https://coolcomponents.co.uk/products/i2c-navkey-7-function-joypad-black)
I thought I'd try something different. I also had an STM32F411 BlackPill (https://shop.pimoroni.com/products/stm32f411-blackpill-development-board?variant=39274213343315)
which has tons of memory and is really cheap.

So I remixed everything, sprinkled a new flavour of my GUI library (https://os.mbed.com/users/duncanFrance/code/SimpleGUI/) on the top and seem to be making 
progress. 

Note the the guts of the applicatio code are in lib/BassMate. I'm using the Model View Presenter pattern, where the Presenter maintains overall app state and 
is responsible for routing UI interactions to the Model and View. Traditional MVC doesn't really work in this kind of product since it relies and
a windowing system routing mouse-clicks directly to on-screen widgets which can then source events.

![The Teensy Board](https://raw.githubusercontent.com/ukmaker/BassMate/master/assets/BassedMate-MainBoard.jpeg)
![The Prototype UI](https://raw.githubusercontent.com/ukmaker/BassMate/master/assets/BassedMate-Top.jpeg)
