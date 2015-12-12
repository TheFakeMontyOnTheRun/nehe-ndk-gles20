Lesson 5
=========
This time, I had a hard time and needed to change things slightly on the Java side.

The changes in the Java side where simple but important: our surface specification wouldn't allow us to have a depth buffer and thus, everything could, depending on the angle, look messed up. The only thing you could do is to deal it from "business logic" side, with "Visible Surface Determination" algorithms. This is a subject for later.

Make sure you understand the order of the vertex specification for each face. I decided to switch to clockwise, so it's easier to read.
Notice you have to specify the faces so that, if you were looking straight at it by spinning the shape, it would have the clockwise order.
eg: faces that normally will appear only behind other things, opposite to you, have their normal (for instance) looking at the same direction as you. Thus, we can safely discard it and save some speed, memory and battery.

This concludes the first batch of NeHe lessons. Now, I must go back and review all lessons, to make sure it's not buggy or misbehaving.

Next up: Texture mapping and god knows what else.
