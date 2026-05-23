/**
 * HelloJS - Axmol JavaScript Example
 * 
 * This is the main entry point for your JavaScript game.
 * Loaded by AppDelegate and executed using QuickJS engine.
 */

// Log a message
console.log("Hello from JavaScript!");

// You can access Axmol engine API here
// Example: Create a simple scene with a label

// Define game configuration
const gameConfig = {
    title: "Hello JavaScript",
    width: 960,
    height: 640,
    fps: 60
};

// Game initialization
function initGame() {
    console.log("Initializing game...");
    console.log("Game Config:", JSON.stringify(gameConfig));
    
    // Your game initialization code here
    // This function is called when the game starts
}

// Game update loop
function updateGame(deltaTime) {
    // This function is called every frame
    // deltaTime is the time elapsed since the last frame in seconds
}

// Game cleanup
function cleanupGame() {
    console.log("Cleaning up game...");
}

// Initialize the game
initGame();

// Export functions for C++ to call
if (typeof module !== 'undefined' && module.exports) {
    module.exports = {
        gameConfig: gameConfig,
        initGame: initGame,
        updateGame: updateGame,
        cleanupGame: cleanupGame
    };
}
