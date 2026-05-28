const Scene = ax.Scene;
const Sprite = ax.Sprite;

global.__axFile = 42;

if (global.__axRunImageSpriteTest) {
    global.__axImageSpriteStage = 1;

class ActionScene extends Scene {
    onEnter() {
        global.__axImageSpriteStage = 2;
        super.onEnter();

        const size = view.getDesignResolutionSize();
        const sprite = Sprite.create('res/Hello.png');
        if (!sprite) {
            global.__axImageSpriteSpriteNull = 1;
            return;
        }

        global.__axImageSpriteStage = 3;
        sprite.setPosition(size.width / 2, size.height / 2);
        this.addChild(sprite);

        const moveAction = moveBy(5, p(100, 600));
        sprite.runAction(moveAction);
        global.__axImageSpriteTest = 1;
    }
}

global._isContextMenuEnable = true;
game.run(
    {
        debugMode: 1,
        showFPS: true,
        frameRate: 60,
        id: 'gameCanvas',
        renderMode: 2,
    },
    function onStart() {
        view.enableRetina(sys.os === sys.OS_IOS);
        view.adjustViewPort(true);

        const width = 720;
        const height = 1280;
        const policy = width > height ? ResolutionPolicy.FIXED_HEIGHT : ResolutionPolicy.FIXED_WIDTH;
        view.setDesignResolutionSize(width, height, policy);
        view.resizeWithBrowserSize(true);

        const scene = new ActionScene();
        if (!scene) {
            global.__axImageSpriteSceneNull = 1;
            return;
        }

        scene.onEnter();
        director.runScene(scene);
    }
);
}
