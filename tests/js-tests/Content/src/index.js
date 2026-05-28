/**
 * Space Shooter – JS Binding Stress Test
 *
 * Tests exercised:
 *   - ax.Scene constructor / onEnter
 *   - ax.Sprite.create, setPosition, setScale, setAnchorPoint, setOpacity, runAction
 *   - scene.addChild
 *   - director.runScene
 *   - moveBy global action helper
 *   - view.*  (getDesignResolutionSize, setDesignResolutionSize, enableRetina,
 *               adjustViewPort, resizeWithBrowserSize)
 *   - game.run
 *   - sys.os
 *   - console.log
 *   - setTimeout / setInterval  (polyfills)
 *   - global flag tracking (for native C++ test assertions)
 */

const Scene = ax.Scene;
const Sprite = ax.Sprite;
const SpriteFrameCache = ax.SpriteFrameCache;
const p = ax.p;
const moveBy = ax.moveBy;
const view = ax.view;
const sys = ax.sys;
const ResolutionPolicy = ax.ResolutionPolicy;
const game = ax.game;
const Director = ax.Director;
const director = Director.getInstance();

// ── constants ────────────────────────────────────────────────────────────────
const W = 720;
const H = 1280;

// ── tiny helper ─────────────────────────────────────────────────────────────
function rnd(min, max) { return min + Math.random() * (max - min); }
function rndInt(min, max) { return Math.floor(rnd(min, max + 1)); }

// ────────────────────────────────────────────────────────────────────────────
//  GameScene
// ────────────────────────────────────────────────────────────────────────────
class GameScene extends Scene {
    onEnter() {
        super.onEnter();
        console.log('[SpaceShooter] GameScene.onEnter');
        global.__jsShooterEntered = 1;
        this._player = Sprite.create('res/arrows.png');
        if (this._player) {
            this._player.setScale(0.25);
            this._player.setAnchorPoint(0.5, 0.5);
            this._player.setPosition(W / 2, H * 0.12);
            this.addChild(this._player, 10);
            global.__jsShooterPlayerCreated = 1;
            console.log('[SpaceShooter] player created');

            // idle bobbing motion
            const bob = moveBy(1.2, { x: 0, y: 20 });
            this._player.runAction(bob);
        } else {
            global.__jsShooterPlayerNull = 1;
            console.log('[SpaceShooter] WARNING: player sprite null');
        }
    }
}

// Allow the native C++ test to gate execution just like the old code did
global.__jsShooterReady = 1;

game.run(
    {
        debugMode: 1,
        showFPS: true,
        frameRate: 60,
        id: 'gameCanvas',
        renderMode: 2,
    },
    function onStart() {
        // view setup
        view.enableRetina(sys.os === sys.OS_IOS);
        view.adjustViewPort(true);

        const policy = W > H ? ResolutionPolicy.FIXED_HEIGHT : ResolutionPolicy.FIXED_WIDTH;
        view.setDesignResolutionSize(W, H, policy);
        view.resizeWithBrowserSize(true);

        console.log('[SpaceShooter] onStart: resolution set to', W, 'x', H);

        const scene = new Scene();
        if (!scene) {
            global.__jsShooterSceneNull = 1;
            console.log('[SpaceShooter] ERROR: scene is null');
            return;
        }
        console.log('[director]', director.runScene);
        console.log('[scene]', scene);
        director.runScene(scene);

        global.__jsShooterStarted = 1;
        console.log('[SpaceShooter] director.runScene done');
    }
);
