package me.hdt.board;

public class LedSetCommand extends TTTCommand {

    public final byte x;

    public final byte y;

    public final byte colorIndex;

    public LedSetCommand(byte x, byte y, byte colorIndex) {
        super(Type.BOARD_SET);

        this.x = x;
        this.y = y;
        this.colorIndex = colorIndex;
    }

}
