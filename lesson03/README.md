Lesson 2
=========
This lesson brings a lot of stuff, so read it carefully!

For this lesson, we needed to provide geometry in the form of Vertex Arrays - it's too soon for Vertex Buffer Objects or anything like this.
Also, I could provide the geometry with the final size, but to illustrate the point, I really wanted to transform the geometry by uploading matrices and performing the 
multiplication inside the shader (also because I'm lazy and didn't want to bother doing it on the CPU, when the GPU do it so well). Thus, two matrices are uploaded to the GPU as uniforms, so that once performs the perspective transformation and other will translate said geometry into it's final position.

Lastly, I decided to split the OpenGL ES code from the NDK code, so it will be easier to read. Most of the time, you can safely ignore the NdkGlue code.

Now, you ask me: what is a uniform? 
It's a piece of data that's constant the whole time, for all the vertices and fragment work. Of course, you must upload the proper type.

Next up: indexed geometry and atributes.
