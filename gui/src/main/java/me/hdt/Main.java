package me.hdt;

import java.io.*;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

public class Main {

    public static volatile boolean running = true;

    public static List<ConfigEntry> config = new LinkedList<>();

    public static void main(String[] args) throws InterruptedException {
        System.out.println("TTT GUI");

        try {

            String cwd = System.getProperty("user.dir");
            System.out.println(String.format("CWD: %s", cwd));

            File file = new File("values.cfg");
            Scanner reader = new Scanner(file);

            while (reader.hasNext()) {
                String line = reader.nextLine();
                String[] parts = line.split(",");

                if (parts.length != 2) {
                    System.out.println(String.format("Invalid config line: %s", line));
                    continue;
                }

                int value = Integer.parseInt(parts[0]);
                int dist = Integer.parseInt(parts[1]);

                ConfigEntry entry = new ConfigEntry();
                entry.value = value;
                entry.distance = dist;

                config.add(entry);
            }
        } catch (FileNotFoundException e) {
            System.out.println("File values.cfg does not exist, creating new one");

            try {
                FileWriter write = new FileWriter("values.cfg");

                write.write("");

                write.flush();
            } catch (IOException ioException) {
                ioException.printStackTrace();
            }
        }

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
