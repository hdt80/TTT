package me.hdt;

import com.sun.jna.Library;
import com.sun.jna.Native;
import me.hdt.board.BoardClearCommand;
import me.hdt.board.LedSetCommand;
import me.hdt.board.ReadGridCommand;
import me.hdt.board.TTTCommand;

import java.util.ArrayDeque;
import java.util.Queue;

public class Board {

    public interface LibTTT extends Library {
        LibTTT instance = Native.load("ttt", LibTTT.class);

        int comm_init(String path);

        int comm_grid_read(byte[] buffer);

        int comm_led_clear(byte commit);

        int comm_led_on(byte col, byte row, byte color);

    }

    public static Queue<TTTCommand> queue = new ArrayDeque<>();

    public static void ledClear(boolean commit) {
        queue.add(new BoardClearCommand(commit));
    }

    public static void ledOn(int col, int row, int color) {
        if (col > 8 || col < 0) {
            return;
        }
        if (row > 8 || row < 0) {
            return;
        }
        if (color > 8 || color < 0) {
            return;
        }

        queue.add(new LedSetCommand((byte) col, (byte) row, (byte) color));
    }

}
