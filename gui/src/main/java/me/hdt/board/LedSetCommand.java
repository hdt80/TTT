package me.hdt.board;

public class LedSetCommand extends TTTCommand {

    public final char x;

    public final char y;

    public final char colorIndex;

    public LedSetCommand(char x, char y, char colorIndex) {
        super(Type.BOARD_SET);

        this.x = x;
        this.y = y;
        this.colorIndex = colorIndex;
    }

}
