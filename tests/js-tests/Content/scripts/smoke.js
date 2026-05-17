// Loaded via FileUtils search path (Content/)
globalThis.__axFile = 21 * 2;

// Sprite render test with image - display on screen
try {
    // Get the director and current scene
    const director = ax.Director.getInstance();
    const scene = director.getRunningScene();
    
    // Create a sprite with spaceship.png image
    const spriteWithImage = ax.Sprite.create('res/spaceship.png');
    
    // Verify sprite was created and has texture
    if (spriteWithImage !== null && spriteWithImage !== undefined && scene !== null) {
        // Set position to center of screen
        spriteWithImage.setPosition(160, 240);
        
        // Set scale to make spaceship visible
        spriteWithImage.setScale(0.5);
        
        // Set anchor point to center
        spriteWithImage.setAnchorPoint(0.5, 0.5);
        
        // Set opacity to fully visible
        spriteWithImage.setOpacity(255);
        
        // Add sprite to scene so it renders on screen
        scene.addChild(spriteWithImage, 1);
        
        // Verify texture was loaded
        const texture = spriteWithImage.getTexture();
        globalThis.__axImageSpriteTest = (texture !== null && texture !== undefined) ? 1 : 0;
        globalThis.__axImageSpriteName = 'spaceship';
    } else {
        globalThis.__axImageSpriteTest = 0;
    }
} catch (e) {
    globalThis.__axImageSpriteTest = -1;
    globalThis.__axImageSpriteError = String(e);
}
