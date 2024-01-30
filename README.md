<h1>Mr.Clobber's backyard game</h1>
<strong>introduction</strong>
<ul>
  <li>
    <h3>Overview</h3>
    <ol>
      <li>Turn-based 4 player strategy game inspired by Tom&Jerry cartoons.</li>
      <li>Players control cat trying to catch mice while avoiding dogs.</li>
      <li>Takes place on 15*15 grid map with objects like walls, traps, power-ups, food, mice and dogs.</li>
      <li>Players take turns moving cats to catch mice and gain points. Most points after 15 rounds win.</li>
    </ol>
  </li>
  <li>
    <h3>Characters</h3>
    <ol>
      <li><strong>Cat</strong> - can move up, down, left and right each turn up to maximum 3 spaces. Lose 1 energy per space moved. Can attack cats or dogs.</li>
      <li><strong>Mice</strong> - 18 worth 1, 2 or 3 points. Move randomly each turn. Cats gain points by landing on mice.</li>
      <li><strong>Dogs</strong> - 4 breads that chase cats. Reduce cat energy and points if win the battle.</li>
      <li><strong>Chocolate</strong> - Landing boosts cat's attack power.at the start of game ten of them is available but if the count of them becomes less than cats the population of chocolates will be increased.</li>
      <li><strong>Fish</strong> - Cats can Gain 2-4 energy points by eating them.</li>
      <li><strong>Traps</strong> - Stepping on release cat's mice or reduces power/energy.</li>
    </ol>
  </li>
  <li>
    <h3>Rules</h3>
    <ol>
      <li><strong>Turn Order</strong> - Players roll dice each round to determine order also it Stays same that round.</li>
      <li><strong>Movement</strong> - On turn, move cat up to 3 spaces in 4 directions. Lose an energy ratio wins. also the player can confirm the movement if he do not like to continue moving.</li>
      <li><strong>Attacking</strong> - if cat encounters enemy cat or dog, battle occurs. Higher power/energy ratio wins else the power and defence point of the fighting cat will be decreased.</li>
      <li><strong>End of Round</strong> - Mice & Dogs reposition randomly after all players move. Food replensishes if needed.</li>
    </ol>
  </li>
  <li>
    <h3>Winning</h3>
    <ol>
      <li>Most mouse points after 15 rounds wins. Ties broken by higher total power/defence point.</li>
      <li>Final scores displayed and saved to a text file named "./save/state.txt"</li>
      <li>You can save and resume the game as well as you want. the data of game will be saved in the "./save/game.dat"</li>
    </ol>
  </li>
</ul>
<h2>Team:</h2>
<a><b>1-</b>Amir Hossein Seddighi</a></br>
<a><b>2-</b>Saeed Samimi Roshkhar</a>
