Lesson 4
=========
While very little seems changed, there are quite a few important changes here. Bear with me:

- Everything now is a instance field of the GLES2Lesson class. This is for the best, since having everything static is asking for trouble.

- More importantly, since we're using VBOs, the vertex data now also has the colour data merged into one single float array. Please notice that I decided to geometry data for the two forms separated (and accept some speed loss) for the benefit of better organization.
In reality, I believe the speed gain would only be significant if I had more stuff happening at a second rendering pass or maybe some expensive shader processing. Or even some expensive property change between each draw call.

Next up: Volumes & Back-face Culling.
