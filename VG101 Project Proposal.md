---
Title: VG101 Project Proposal
Author: Project Group 30
---

## 1. Project Group Number: Group 30

## 2. Members:
|Name|Name in Hanzi|Student ID|
|----|-------------|----------|
|Binhao Qin|秦彬皓|519370910092|
|Jingbo Qu|瞿敬柏|519370910091|
|Wenqi Cao|曹文琦|519370910111|

## 3. Project Name: C++-based Rhythm Game

## 4. Intended Language: C++ (May involve little parts with MATLAB)

## 5. Project Summary
This project aims to make a rhythm game by C++ and OpenGL, with a Graphical User Interface provided. *An advanced goal is to make a converter that can convert .midi music file into .osu like track file with MATLAB. This part may not be achieved if time is not enough.*

## 6. Motivation
Music games can help players relax and increase their sense of music. Developing a simple music game is a challenge as well as a meaningful task for us. We can learn useful concepts and applications of C++ that may not be learned in class while developing. (e.g. class, virtual function, JSON file, OpenGL, etc.)

## 7. Tentative Design
### 7.1 Features and Descriptions
- Vertical Scrolling Rhythm Games
- Simple and intuitive gameplay
- Through tapping, follow the rhythm to play song
- Experience touching emotion through music
- Relax and Refresh after playing the game
### 7.2 Data Dictionary or Datagram 
>PIC

## 8. Expected Outcome
### 8.1  Bottom-line 
- Games can be played through Command Line
- As the music plays, "0"s will appear and move downward from the top of the screen in 4 or 6 columns
- Players can tap space as "0" hit the line at the bottom of the screen
- Handle one piece of music score that the game can play
- Hitting the space key in time will gain points
- Total scores can be correctly calculated at the end of the game
### 8.2 Expected
- Games can be played in GUI (Graphical User Interface)
- As the music plays, notes will appear and move downward from the top of the screen in 4 or 6 columns
- Handle more than one piece of music score that the game can play
- Players can tap (press) corresponding keys bound to several columns as notes hit the line at the bottom of the screen.
- Hitting the notes more accurately increases the final score after completing the whole song.
### 8.3 Potential
- Cover, Contents and Play/Stop/Exit are included in GUI (Graphical User Interface)
- As the music plays, notes are falling from the top of the screen in 4 or 6 columns
- Correctness effect, combo effect and falling effect of the notes are included in GUI 
- Use algorithm to transform midi to music scores that the game can play
- Players tap or hold (press or long-press) corresponding keys bound to several columns according to the music as notes hit the line at the bottom of the screen.
- Update and calculate scores while playing
- Calibration before playing the game

## 9. Timetable
|Time|Things to do|
|----|-------------|
|6/25 —— 7/ 3|Learn as much as possible|
|7/ 4 —— 7/12|Coding the interface|
|7/12 —— 7/24|Read the midi file|
|7/25 —— 8/ 2|System Designing|

## 10. Extra preparations

- Learning C++ in advance
- Learning JSON file
- Learning OpenGL in advance

## 11. Task assignment
|Name|Task|
|----|-------------|
|Binhao Qin| System integrating the whole program and read the midi file|
|Jingbo Qu| Designing the Graphical User Interface|
|Wenqi Cao| Designing the inner logic|
