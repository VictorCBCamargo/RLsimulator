This file aims to explain the parameters to be used in running the simulations and their possible values. The parameters may be set in the file named "input.txt".

Parameter definition:
=====================

(sde) The first parameter refers to the standard deviation estimator selected to simulate. The possible values are: "1" to select the SD1 - standard deviation estimator; "2" to select the SD2 - standard deviation estimator; "3" to select the SD3 - standard deviation estimator; "4" to select the SD4 - standard deviation estimator; "5" to select the SD5 - standard deviation estimator.
(j) number of practitioners (each one with a random estimate of the in-control process mean and standard deviation).
(k) number of repetitions of taking samples to certain RL.

Warnings:
=========

- It is important to remark that the space between ":" and the value is mandatory.
- Make sure to save the file of results after running a round of the simulation and before starting a new round.