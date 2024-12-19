# Project Lephon
Course Project for CS4187 Computer Vision for Interactivity, Semester A 2024/25, City University of Hong Kong.

## Overview

Project Lephon is an interactive rhythm game developed using openFrameworks. It combines hand tracking with engaging gameplay to provide a dynamic and immersive experience. Players interact with the game by moving their hands in front of a webcam to tap and hold notes in sync with music videos.

## Features

- **Hand Tracking**: Utilizes a pre-trained neural network for real-time hand detection using the system's webcam.
- **Rhythm-Based Gameplay**: Offers two types of notes—tap notes and hold notes—that players must interact with at precise timings.
- **Dynamic Visuals**: Displays shrinking circles and images for notes to provide visual cues aligned with the music.
- **Scoring System**: Rewards players with "GREAT!" or "MISS" feedback based on their accuracy.
- **Chart Creation Mode**: Allows users to create custom note charts by recording their hand movements while watching a video.

## Demo
https://github.com/user-attachments/assets/8889f7b8-b901-40df-af9d-44d51a198b3b

## Build Instructions

### Requirements

- **Hardware**: A computer with a webcam.
- **Software**:
  - OS: Tested on macOS Sequoia (version 15.2), but should work on other platforms as well.
  - openFrameworks version 0.12.0.
    - Addons: `ofxOpenCv`, `ofxCv`, `ofxGui`.

### Setup

1. **Clone the Repository**: Download or clone the project to your local machine.
2. **Data Files**: Ensure the following files are placed in the `bin/data` directory:
   - `cross-hands-tiny-prn.cfg` and `cross-hands-tiny-prn.weights` for hand detection.
   - Images: `hand.png`, `tap.png`, `hold.png` for visual elements.
3. **Build the Project**: Open the project in your IDE and compile it using openFrameworks.

## Running the Game

1. **Start the Application**: Run the compiled executable.
2. **Main Menu**: Upon launch, you'll see the main menu where you can choose to:
   - **Play Game**: Start playing with an existing note chart.
   - **Create Chart**: Create a new note chart for a video.
3. **Selecting a Video**: Choose an `.mp4` video file when prompted.
4. **Gameplay**:
   - Make sure your hand is visible to the webcam.
   - **Hand Icon**: A hand icon will follow your hand movements on the screen.
   - **Tap Notes**: Quickly move your hand over the tap notes as they appear.
   - **Hold Notes**: Hold your hand over the hold notes for their duration.
5. **Feedback**:
   - A "GREAT!" message indicates a successful hit.
   - A "MISS" message appears if you miss a note.

### Creating a Chart

1. **Enter Chart Creation Mode**: Select "Create Chart" from the main menu.
2. **Recording Notes**:
   - **Tap Notes**: Press any key (except the spacebar) to place a tap note at the current time and hand position.
   - **Hold Notes**:
     - Press and hold the spacebar to start a hold note.
     - Release the spacebar to end the hold note.
3. **Saving the Chart**: The note chart will be saved as a `_gen.txt` file alongside your video.

## Implementation Details

### Hand Detection

- **OpenCV DNN Module**: The application uses OpenCV's DNN module to load a pre-trained YOLO model (`cross-hands-tiny-prn`) for hand detection.
- **Processing**:
  - Captures frames from the webcam and processes them to detect hands.
  - Updates the position of the hand icon based on detection results.

### Note Classes

- **Note Base Class**:
  - Contains common attributes like `x`, `y`, `time`, and methods for updating and drawing.
  - Abstract methods `update`, `draw`, `isGreat`, `isFinished`, and `isStarted` are implemented in derived classes.

- **TapNote**:
  - Represents a single tap note.
  - Displays `tap.png` when not hit.
  - Draws a circle that shrinks as the note's time approaches, providing a visual countdown.

- **HoldNote**:
  - Represents a note that must be held over a duration.
  - Displays `hold.png` and a progress indicator showing how long the note has been held.
  - Tracks the `holdTime` to determine if the note was successfully completed.

### Game Modes

- **PlayGame**:
  - Manages the main gameplay.
  - Loads notes from a chart file (`_gen.txt` or `.txt`).
  - Updates and draws active notes based on the current time.
  - Checks for note hits and provides feedback.

- **CreateChart**:
  - Allows players to create custom note charts.
  - Records the timing and position of tap and hold notes based on key presses and hand position.
  - Saves the recorded notes to a `_gen.txt` file for future gameplay.

### File Structure

- **Main Application**:
  - `ofApp.cpp` and `ofApp.h`: Handle the overall application setup, including webcam initialization and hand detection.
- **Modes**:
  - `BaseMode`: An abstract class that other modes inherit from.
  - `MainMenu`, `SelectSong`, `PlayGame`, `CreateChart`, `EndScreen`: Classes representing different states of the application.
- **Notes**:
  - `Note.h`: Contains the `Note`, `TapNote`, and `HoldNote` class definitions.
- **Resources**:
  - **Models**: Neural network configuration and weights files for hand detection.
  - **Images**: Visual assets like `hand.png`, `tap.png`, and `hold.png`.

## Future Improvements
- Improve hand detection accuracy and robustness.
- Add support for multiple hand detection.
- Allow users to adjust audio offset.

## Acknowledgements
- The hand detection model was obtained from the [cansik/yolo-hand-detection](https://github.com/cansik/yolo-hand-detection) repository.

- The two music videos used in the game are:
  - [Testify](https://www.youtube.com/watch?v=xkUN_9HFNPg)
  - [Last | Eternity](https://www.youtube.com/watch?v=OCbVjezDxGw)

- The icons for hand, tap note, and hold note were sourced from [Flaticon](https://www.flaticon.com/).
