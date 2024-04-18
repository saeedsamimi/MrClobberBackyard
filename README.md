# 🐱 **Mr. Clobber's Backyard Game** 🐱

## Overview
- Turn-based 4 player strategy game inspired by Tom & Jerry cartoons.
- Players control cat trying to catch mice while avoiding dogs.
- Takes place on 15x15 grid map with objects like walls, traps, power-ups, food, mice, and dogs.
- Players take turns moving cats to catch mice and gain points. Most points after 15 rounds win.

---

## Characters
1. **🐾 Cat** - Can move up, down, left, and right each turn up to a maximum of 3 spaces. Lose 1 energy per space moved. Can attack cats or dogs.
2. **🐭 Mice** - 18 worth 1, 2, or 3 points. Move randomly each turn. Cats gain points by landing on mice.
3. **🐶 Dogs** - 4 breeds that chase cats. Reduce cat energy and points if they win the battle.
4. **🍫 Chocolate** - Landing boosts cat's attack power. At the start of the game, ten of them are available, but if the count becomes less than cats, the population of chocolates will be increased.
5. **🐟 Fish** - Cats can gain 2-4 energy points by eating them.
6. **🚧 Traps** - Stepping on them releases cat's mice or reduces power/energy.

---

## Rules
1. **🎲 Turn Order** - Players roll dice each round to determine order, and it stays the same that round.
2. **👣 Movement** - On the turn, move cat up to 3 spaces in 4 directions. Lose an energy ratio wins. The player can confirm the movement if they do not want to continue moving.
3. **⚔ Attacking** - If a cat encounters an enemy cat or dog, a battle occurs. Higher power/energy ratio wins; otherwise, the power and defense point of the fighting cat will be decreased.
4. **🔄 End of Round** - Mice & Dogs reposition randomly after all players move. Food replenishes if needed.

---

## Winning
1. Most mouse points after 15 rounds win. Ties broken by a higher total power/defense point.
2. Final scores displayed and saved to a text file named `./save/state.txt`.
3. You can save and resume the game as well as you want. The game data will be encrypted and saved in "./save/game.dat".

## Online Storage
- You can save your maps on server and use them on everywhere. You need to run fill your credentials in `networking/profile.conf` and then run online_map Program.
---

## Team:
- [**1-** Amir Hossein Seddighi](#)
- [**2-** Saeed Samimi Roshkhar](#)
