package me.hdt.board;

/**
 * Command used for initalizing a connection do the device
 */
public class InitializeCommand extends TTTCommand {

    public InitializeCommand() {
        super(Type.INITITALIZE);
    }

}
