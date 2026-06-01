// require config

var HelloWorldScene = ax.Scene.extend({
    ctor: function() {
        this._super();
        var size = ax.director.getWinSize();

        // add background image
        var sprite = ax.Sprite.create("HelloWorld.png");
        sprite.setPosition(size.width / 2, size.height / 2);
        this.addChild(sprite, 0);

        // add HelloWorld label
        var label = ax.Label.createWithSystemFont("Hello World", "Arial", 40);
        label.setPosition(size.width / 2, size.height / 2 + 200);
        this.addChild(label, 1);
    }
});

var scene = new HelloWorldScene();
ax.director.runWithScene(scene);
