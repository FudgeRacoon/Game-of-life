<h1>Introduction</h1>
<p>
  This is a <code>c</code> program based on the famous simulation <a href="https://en.wikipedia.org/wiki/Conway's_Game_of_Life" target = "_blank">Conway's game of life</a>. The     simulation works by changging the state of cells from living to dead and vice verse every frame according to a predefined ruleset:
</p>
  <ul>
    <li>Any live cell with fewer than two live neighbours dies, as if caused by under-population.</li>
    <li>Any live cell with two or three live neighbours lives on to the next generation.</li>
    <li>Any live cell with more than three live neighbours dies, as if by over-population.</li>
    <li>Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.</li>
  </uL>

<h1>How the program works</h1>
<p>
  The program works by passing in the size the size of the window in the command-line and the filepath of the starting state and then the simulation begins. To exit the program     press <code>Q</code> or simply click on the exit button, upon quitting of the program a saved file state is generated that stores the state of each cell at the time the           program has ended. A maximum of 10 files can be saved then the most recent file is overwritten.
</p>
<h1>Building and running the program</h1>
<p>
  A makefile is included with this program, to compile the program simply type in the command-line <code>make compile</code>. To run the program type <code>make run</code>
</p>
