package me.hdt.board;

/**
 * Command for when the board will be cleared
 */
public class BoardClearCommand extends TTTCommand {

    /**
     * Will the clear be committed and the LEDs updated?
     */
    public final boolean commit;

    public BoardClearCommand(boolean commit) {
        super(Type.BOARD_CLEAR);

        this.commit = commit;
    }

}
