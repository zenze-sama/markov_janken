#  Markov Janken
A rock paper scissor (janken) game that tries to predict your moves.

This uses a [Markov Chain](https://en.wikipedia.org/wiki/Markov_chain) to predict your next move based on your recent move history. In this particular case, the last two moved because humans dont tend to think farther in future than that. 

For Example: If the player's last two moves were Rock and Paper, the algorithm checks how frequently the player chose Rock, Paper, or Scissors after the "RP" sequence. The AI then predicts the most likely next move and counters it (e.g., if the player usually follows "RP" with Scissors, then the AI plays Rock) and if no pattern exists yet, its random because why not!

Over time this "should" adapt to the players subconscious decisions and get a 100% win-rate. I am using "should" because humans are not dumb, we will obviously change our strategy and pay attention to our monitor if a program wins 5 times in a row.

Try it: https://zenze-sama.github.io/markov-janken-pygame/

I intend to refine this a little more, so I'll make a changelog here.

<details>
<summary>Changelog:</summary>

- 1
   - Made the intial commit. No optimizations yet.

- 2
   - Made counterMove, to get a counter move, duh.
   - Made getFrequencyBasedMove, this makes it so that in early game, when no patterns are found, it is not relied on a random move, rather we can count the most common move from the player in playerHistory and use that, if empty, use random move.
   - Made isRepeatingPattern and getAntiHumanMove, it checks if the last move is repeated 3 times, if yes then it refrains from countering the same move again because humans dont use the same move 3 times a row. But this makes a stupid bug, if you spam just 1 move consecutively you will trick the ai into always picking the wrong move against you. So essentially beating the system (The idea sounded better in my head, trust). Might remove this later. Also the way I have implemented this is also very stupid so def getting removed next patch.

- 3
   - Upgraded getMarkovMove with a spam detection logic. Also one little thing, either I am really predictable or this is actualy really good at predicting stuff. In my test runs I have been unable to beat this even though I know how it works.
   
- 4
   - Removed scrolling text because its stupid.
   - Score display on top, qol fr
   - Hardcoded a 1 second delay when the wrong move error is displayed for absolutely no reason :100:

- 5
   - Made a player history "dynamic board" that changes with every turn! Aesthetic fr.
   - You can now see what moves of yours markov predicted by using the -lp flag (short for learned patterns). Btw this entire file output system, like making the markov_patterns.txt file and saving was made with Deepseek, I tried making it myself but wasted 5 hours on it for some reason :100: .
    
 - 6 
   - [Rewrote entire thing in python](https://github.com/zenze-sama/markov-janken-pygame) and deployed on web with wasm

</details>
