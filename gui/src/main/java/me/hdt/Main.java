package me.hdt;

public class Main {

    public static volatile boolean running = true;

    public static void main(String[] args) throws InterruptedException {
        System.out.println("TTT GUI");

        UpdateThread thread = new UpdateThread();
        thread.start();

        GuiThread gui = new GuiThread(thread);
        gui.start();

        while (running) {
            Thread.onSpinWait();
        }

        gui.join();
        thread.join();
    }

}
