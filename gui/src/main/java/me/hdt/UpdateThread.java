package me.hdt;

import me.hdt.board.BoardClearCommand;
import me.hdt.board.LedSetCommand;
import me.hdt.board.TTTCommand;

public class UpdateThread extends Thread {

    private char[] board;

    private boolean running = false;

    public char[] getBoard() {
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

        board = new char[64];
        running = true;

        while (Main.running) {
            TTTCommand cmd = Board.queue.poll();

            if (cmd != null) {
                System.out.println(String.format("Got a command to execute: %s", cmd.toString()));
                if (cmd.type == TTTCommand.Type.INITITALIZE) {
                    int a = 1;
                } else if (cmd.type == TTTCommand.Type.BOARD_CLEAR) {
                    BoardClearCommand argCmd = (BoardClearCommand) cmd;
                    Board.LibTTT.instance.comm_led_clear((char) (argCmd.commit ? 0x01 : 0x00));
                } else if (cmd.type == TTTCommand.Type.BOARD_SET) {
                    LedSetCommand argCmd = (LedSetCommand) cmd;
                    Board.LibTTT.instance.comm_led_on(argCmd.x, argCmd.y, argCmd.colorIndex);
                } else {
                    System.err.println(String.format("Invalid command type %s", cmd.toString()));
                }
            } else {
                Board.LibTTT.instance.comm_grid_read(this.board);
                try {
                    Thread.sleep(20);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

}
