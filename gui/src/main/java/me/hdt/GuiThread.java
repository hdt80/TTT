package me.hdt;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class GuiThread extends Thread {
    private JFrame frame;

    private final UpdateThread updateThread;

    private final JPopupMenu contextMenu;
    private final JButton[] buttons = new JButton[64];
    private Point selectedGridCoord = null;

    public GuiThread(UpdateThread thread) {
        this.updateThread = thread;

        contextMenu = new JPopupMenu();

        JMenuItem redColor = new JMenuItem();

        String[] colors = {"Red", "Green", "Blue"};
        for (int i = 0; i < colors.length; ++i) {
            JMenuItem item = new JMenuItem(new ContextMenuAction(i));
            item.setText(colors[i]);
            contextMenu.add(item);
        }

    }

    public void run() {
        frame = new JFrame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(800, 800);
        frame.setFocusable(true);

        GridLayout grid = new GridLayout(8, 8);
        frame.setLayout(grid);

        frame.addKeyListener(new CloseListener(this));

        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                buttons[(i * 8) + j] = new JButton("Loading");
                JButton b = buttons[(i * 8) + j];
                b.setName(String.format("grid %d %d", i, j));
                b.setFocusable(false);

                b.addMouseListener(new ButtonClickListener(i * 8 + j));
                frame.add(b);
            }
        }

        frame.setVisible(true);

        while (Main.running) {
            if (!updateThread.isRunning()) {
                try {
                    System.out.println("UpdateThread is not running, waiting 500ms for that");
                    Thread.sleep(500);
                    continue;
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            char[] board = updateThread.getBoard();

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    JButton button = getGridButton(i, j);
                    if (button == null) {
                        System.err.println(String.format("Missing button for %d %d", i, j));
                        continue;
                    }

                    char value = board[i * 8 + j];

                    button.setText(String.format("0x%02x", (int) value));
                    if (value > 0) {
                        button.setBackground(Color.GREEN);
                    } else {
                        button.setBackground(Color.GRAY);
                    }
                }
            }
        }
    }

    private JButton getGridButton(int row, int col) {
        String name = String.format("grid %d %d", row, col);
        for (Component comp : frame.getContentPane().getComponents()) {
            if (comp.getName() == null) {
                continue;
            }
            if (comp.getName().equals(name)) {
                return (JButton) comp;
            }
        }
        return null;
    }

    private Point parseGridName(String name) {
        if (!name.startsWith("grid")) {
            return null;
        }

        String[] parts = name.split(" ");
        if (parts.length != 3) {
            return null;
        }

        String xPart = parts[1];
        String yPart = parts[2];

        int x = Integer.parseInt(xPart);
        int y = Integer.parseInt(yPart);

        return new Point(x, y);
    }

    private static class CloseListener implements KeyListener {
        private final GuiThread thread;

        public CloseListener(GuiThread thread) {
            this.thread = thread;
        }

        public void keyPressed(KeyEvent keyEvent) {
            if (keyEvent.getKeyCode() == KeyEvent.VK_ESCAPE) {
                if (keyEvent.isShiftDown()) {
                    thread.frame.dispose();
                    Main.running = false;
                } else {
                    Board.ledClear(true);
                }
            }
        }

        public void keyReleased(KeyEvent keyEvent) { }
        public void keyTyped(KeyEvent keyEvent) { }
    }

    private class ContextMenuAction extends AbstractAction {
        private final int colorIndex;

        public ContextMenuAction(int index) {
            this.colorIndex = index;
        }

        @Override
        public void actionPerformed(ActionEvent actionEvent) {
            if (selectedGridCoord == null) {
                System.out.println(String.format("Cannot handle mouseClick, no selected coord"));
                return;
            }

            System.out.println(String.format("Turning LED on (%d, %d) to %d",
                    selectedGridCoord.x, selectedGridCoord.y, colorIndex));

            Board.ledOn(selectedGridCoord.x, selectedGridCoord.y, colorIndex);

            System.out.println("Action on contextMenu");
        }
    }

    private class ContextMenuClickListener implements MouseListener {
        private final int colorIndex;

        public ContextMenuClickListener(int colorIndex) {
            this.colorIndex = colorIndex;
        }

        public void mouseClicked(MouseEvent mouseEvent) {
        }

        public void mousePressed(MouseEvent mouseEvent) { }
        public void mouseReleased(MouseEvent mouseEvent) { }
        public void mouseEntered(MouseEvent mouseEvent) { }
        public void mouseExited(MouseEvent mouseEvent) { }
    }

    private class ButtonClickListener implements MouseListener {
        private final int index;

        public ButtonClickListener(int index) {
            this.index = index;
        }

        public void mouseClicked(MouseEvent mouseEvent) {
            System.out.println(String.format("Select button index %d", index));

            int mx = mouseEvent.getX();
            int my = mouseEvent.getY();

            Component comp = frame.getContentPane().getComponentAt(mx, my);
            if (comp == null) {
                System.out.println(String.format("No component found at (%d, %d)", mx, my));
                return;
            }
            if (!(comp instanceof JButton)) {
                System.out.println(String.format("No JButton at (%d, %d)", mx, my));
                return;
            }

            String buttonName = ((JButton) comp).getName();
            Point gridCoords = parseGridName(buttonName);
            if (gridCoords == null) {
                System.out.println(
                    String.format("JButton at (%d, %d) is not a grid button %s",
                        mx, my, buttonName)
                );
                return;
            }

            selectedGridCoord = gridCoords;

            if (mouseEvent.getButton() == MouseEvent.BUTTON1) {
                System.out.println("A");
            } else if (mouseEvent.getButton() == MouseEvent.BUTTON3) {
                contextMenu.show(frame, mx, my);
                System.out.println(String.format("Showing context menu at (%d, %d)", mx, my));
            }
        }

        public void mousePressed(MouseEvent mouseEvent) { }
        public void mouseReleased(MouseEvent mouseEvent) { }
        public void mouseEntered(MouseEvent mouseEvent) { }
        public void mouseExited(MouseEvent mouseEvent) { }
    }
}
