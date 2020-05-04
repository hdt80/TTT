package me.hdt.board;


public abstract class TTTCommand {

    public enum Type {
        UNKNOWN,

        INITITALIZE,

        BOARD_CLEAR,

        BOARD_SET,

        GRID_READ

    };

    public final Type type;

    public TTTCommand(Type type) {
        this.type = type;
    }

}

