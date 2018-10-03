# AI Bot for the game of Tak <div style="text-align: right"> 
[![Build Status](https://travis-ci.org/akshittyagi/TakBot.svg?branch=master)](https://travis-ci.org/akshittyagi/TakBot)
</div>

## Running
This repo contains the code for an AI bot, for playing a game of Tak. <a href="https://en.wikipedia.org/wiki/Tak_(game)">Tak</a> is a recent game taken from the The Kingkiller chronicle. It is a board game which requires some strategic play. The bot we have developed is part of an assignment for <a href="http://www.cse.iitd.ac.in/~mausam/courses/col333/autumn2016/">COL333</a>: Intro. to AI course.

To make the bot play against itself call the following scripts in the same order:

`./scripts/server.sh <PortNo> `


`./scripts/player1.sh <PortNo> `


`./scripts/player2.sh <PortNo> `

## Algorithm
TakBot uses a minimax tree to make decisions on the moves it wants to make in a given scenario. It rates all the possible future states it can see upto four moves deep within the Tree and then selects the move which maximises this utility function.

We have made the TakBot play against itself in multiple different configurations, similar to what AlphaGo does to train without a human at the other end. It learns the features and their weights in the utility function thus getting better at rating these future states heuristically.

## Future Work
We plan on using a augmented Monte Carlo-* Mininax Search in the future as is done <a href="https://repository.tudelft.nl/islandora/object/uuid:72ae1c45-5b8f-4650-9b7e-e02c45dec4a4/datastream/OBJ/download.">here</a>
  
