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

console.log('[SpaceShooter] JS binding test start');

const Scene  = ax.Scene;
const Sprite = ax.Sprite;
const SpriteFrameCache = ax.SpriteFrameCache;

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

        // ── load resources ──────────────────────────────────────────────────
        SpriteFrameCache.getInstance().addSpriteFramesWithFile('res/enemy_packed.plist');
        SpriteFrameCache.getInstance().addSpriteFramesWithFile('res/bullets/new_bullet.plist');

        // ── starfield (background scrolling) ────────────────────────────────
        this._stars = [];
        for (let i = 0; i < 30; i++) {
            const star = Sprite.create('res/spaceship.png');
            if (!star) continue;

            const s = rnd(0.04, 0.12);
            star.setScale(s);
            star.setOpacity(Math.floor(rnd(60, 180)));
            star.setPosition(rnd(0, W), rnd(0, H));
            star.setAnchorPoint(0.5, 0.5);
            this.addChild(star, 0);
            this._stars.push({ node: star, speed: rnd(40, 120) });
        }

        // ── player ship ─────────────────────────────────────────────────────
        this._player = Sprite.create('res/spaceship.png');
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

        // ── enemies (5 waves, staggered) ────────────────────────────────────
        this._enemies = [];
        const cols = 4;
        const rows = 2;
        for (let row = 0; row < rows; row++) {
            for (let col = 0; col < cols; col++) {
                // Use random enemy from plist
                const enemyName = 'enemy' + rndInt(1, 7) + '.png';
                const enemy = Sprite.createWithSpriteFrameName(enemyName);
                if (!enemy) {
                    console.log('[SpaceShooter] WARNING: enemy sprite frame not found:', enemyName);
                    continue;
                }

                enemy.setScale(0.8); // Adjust scale since they are smaller in plist
                enemy.setAnchorPoint(0.5, 0.5);
                const ex = W * 0.15 + col * (W * 0.23);
                const ey = H * 0.70 + row * 120;
                enemy.setPosition(ex, ey);
                this.addChild(enemy, 5);

                // sweep action
                const sweep = moveBy(2.5 + row * 0.3, { x: rnd(-80, 80), y: -200 });
                enemy.runAction(sweep);
                this._enemies.push(enemy);
            }
        }
        global.__jsShooterEnemyCount = this._enemies.length;
        console.log('[SpaceShooter] enemies spawned:', this._enemies.length);

        // ── bullets ─────────────────────────────────────────────────────────
        this._bullets = [];

        // Fire a burst of 5 bullets, 0.4 s apart
        for (let i = 0; i < 5; i++) {
            setTimeout(() => {
                this._fireBullet();
            }, 400 + i * 400);
        }

        // ── score label (represented via a Sprite acting as HUD) ─────────────
        const hud = Sprite.create('res/spaceship.png');
        if (hud) {
            hud.setScale(0.06);
            hud.setAnchorPoint(0.0, 1.0);
            hud.setOpacity(200);
            hud.setPosition(20, H - 20);
            this.addChild(hud, 20);
        }

        // ── test signal: everything initialised ─────────────────────────────
        global.__jsShooterInited = 1;
        console.log('[SpaceShooter] scene fully initialised');
    }

    // spawn a bullet from the player position
    _fireBullet() {
        // Use bullet from plist
        const bullet = Sprite.createWithSpriteFrameName('bullet_1.png');
        if (!bullet) {
            console.log('[SpaceShooter] WARNING: bullet sprite frame not found: bullet_1.png');
            return;
        }

        bullet.setScale(0.5); // Adjust scale
        bullet.setAnchorPoint(0.5, 0.5);
        // default player position as fallback
        bullet.setPosition(W / 2, H * 0.12);
        bullet.setOpacity(230);
        this.addChild(bullet, 8);

        // fly upwards
        const fly = moveBy(1.5, { x: 0, y: H * 0.85 });
        bullet.runAction(fly);
        this._bullets.push(bullet);

        global.__jsShooterBulletsFired = (global.__jsShooterBulletsFired || 0) + 1;
        console.log('[SpaceShooter] bullet fired, total:', global.__jsShooterBulletsFired);
    }
}

// ────────────────────────────────────────────────────────────────────────────
//  Boot sequence (mirrors the existing test harness pattern)
// ────────────────────────────────────────────────────────────────────────────
global.__axFile = 42;

// Allow the native C++ test to gate execution just like the old code did
global.__jsShooterReady = 1;

game.run(
    {
        debugMode  : 1,
        showFPS    : true,
        frameRate  : 60,
        id         : 'gameCanvas',
        renderMode : 2,
    },
    function onStart() {
        // view setup
        view.enableRetina(sys.os === sys.OS_IOS);
        view.adjustViewPort(true);

        const policy = W > H ? ResolutionPolicy.FIXED_HEIGHT : ResolutionPolicy.FIXED_WIDTH;
        view.setDesignResolutionSize(W, H, policy);
        view.resizeWithBrowserSize(true);

        console.log('[SpaceShooter] onStart: resolution set to', W, 'x', H);

        const scene = new GameScene();
        if (!scene) {
            global.__jsShooterSceneNull = 1;
            console.log('[SpaceShooter] ERROR: scene is null');
            return;
        }

        scene.onEnter();
        director.runScene(scene);

        global.__jsShooterStarted = 1;
        console.log('[SpaceShooter] director.runScene done');
    }
);
