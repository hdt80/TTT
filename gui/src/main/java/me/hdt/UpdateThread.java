package me.hdt;

import me.hdt.board.BoardClearCommand;
import me.hdt.board.LedSetCommand;
import me.hdt.board.TTTCommand;

import java.util.LinkedList;

public class UpdateThread extends Thread {

    private byte[] board;

    private boolean running = false;

    private LinkedList<byte[]> linkedBoard = new LinkedList<>();

    public byte[] getBoard() {
        return board;
    }

    public boolean isRunning() {
        return running;
    }

    public void run() {
        running = false;

        int success = Board.LibTTT.instance.comm_init("/dev/ttyUSB0");
        if (success != 0) {
            System.err.println("Failed to connect to target board");
        }

        board = new byte[64];
        running = true;

        byte[] currentBoard = new byte[64];

        while (Main.running) {
            TTTCommand cmd = Board.queue.poll();

            if (cmd != null) {
                //System.out.println(String.format("Got a command to execute: %s", cmd.toString()));
                if (cmd.type == TTTCommand.Type.INITITALIZE) {
                    int a = 1;
                } else if (cmd.type == TTTCommand.Type.BOARD_CLEAR) {
                    BoardClearCommand argCmd = (BoardClearCommand) cmd;
                    Board.LibTTT.instance.comm_led_clear((byte) (argCmd.commit ? 0x01 : 0x00));
                } else if (cmd.type == TTTCommand.Type.BOARD_SET) {
                    LedSetCommand argCmd = (LedSetCommand) cmd;
                    Board.LibTTT.instance.comm_led_on(argCmd.x, argCmd.y, argCmd.colorIndex);
                } else {
                    System.err.println(String.format("Invalid command type %s", cmd.toString()));
                }
            } else {
                Board.LibTTT.instance.comm_grid_read(currentBoard);

                board = currentBoard;

                linkedBoard.add(currentBoard);

                try {
                    Thread.sleep(20);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

}
