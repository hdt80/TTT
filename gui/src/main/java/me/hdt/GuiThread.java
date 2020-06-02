package me.hdt;

import me.hdt.util.BoardUtil;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class GuiThread extends Thread {
    private JFrame frame;

    private final UpdateThread updateThread;

    private final JPopupMenu contextMenu;
    private final JButton[] buttons = new JButton[64];
    private Vec2i selectedGridCoord = null;

    private final List<Vec2i> selectedSquares = new LinkedList<>();

    private final Border highlightedBorder = BorderFactory.createLineBorder(Color.MAGENTA, 3);
    private final Border defaultBorder = BorderFactory.createLineBorder(Color.BLACK, 1);

    public GuiThread(UpdateThread thread) {
        this.updateThread = thread;

        contextMenu = new JPopupMenu();

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

            byte[] board = updateThread.getBoard();

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    JButton button = getGridButton(i, j);
                    if (button == null) {
                        System.err.println(String.format("Missing button for %d %d", i, j));
                        continue;
                    }

                    byte value = board[i * 8 + j];
                    int v = Byte.toUnsignedInt(value);
                    int index = (v & 0xE0) >> 5;

                    button.setText(String.format("%d", index));

                    if (value != 0) {
                        button.setBackground(Color.GREEN);
                        //System.out.println(String.format("(%d, %d) has %d/%d/%d", i, j, value, v, index));
                    } else {
                        button.setBackground(Color.GRAY);
                    }

                    if (selectedGridCoord != null && selectedGridCoord.getX() == i && selectedGridCoord.getY() == j) {
                        button.setBorder(highlightedBorder);
                    } else {
                        button.setBorder(defaultBorder);
                    }
                }
            }

            if (selectedGridCoord != null && selectedSquares.size() > 0) {
                Graphics2D gr = (Graphics2D) frame.getGraphics();
                if (gr == null) {
                    System.out.println("Missing Graphics2D to draw lines");
                    continue;
                }

                JButton selectedButton = getGridButton(selectedGridCoord.getX(), selectedGridCoord.getY());
                if (selectedButton == null) {
                    System.out.println(String.format("Missing JButton for selectedGridCoord"));
                    continue;
                }

                Stroke prevStroke = gr.getStroke();
                gr.setStroke(new BasicStroke(2));

                Vec2i selectedCenter = getButtonCenter(selectedButton);

                // Avoid concurrent modification with accessing and adding in another thread
                ArrayList<Vec2i> squares = new ArrayList<>(selectedSquares);
                for (Vec2i square : squares) {
                    int gx = square.getX();
                    int gy = square.getY();

                    JButton squareButton = getGridButton(gx, gy);
                    if (squareButton == null) {
                        System.err.println(String.format("Missing squareButton %d %d", gx, gy));
                        continue;
                    }

                    Vec2i center = getButtonCenter(squareButton);
                    gr.drawLine(selectedCenter.getX(), selectedCenter.getY(), center.getX(), center.getY());

                    int cx = (selectedCenter.getX() + center.getX()) / 2;
                    int cy = (selectedCenter.getY() + center.getY()) / 2;

                    double dist = BoardUtil.distance(selectedGridCoord, square);

                    gr.drawString(String.format("%.2f", dist), cx, cy);
                }

                gr.setStroke(prevStroke);
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

    private Vec2i getButtonCenter(JButton button) {
        int width = button.getWidth();
        int height = button.getHeight();
        int bx = button.getX();
        int by = button.getY();

        int lineToX = bx + (width / 2);
        int lineToY = by + (height / 2);

        return new Vec2i(lineToX, lineToY);
    }

    private Vec2i parseGridName(String name) {
        if (!name.startsWith("grid")) {
            return null;
        }

        String[] parts = name.split(" ");
        if (parts.length != 3) {
            return null;
        }

        String xPart = parts[1];
        String yPart = parts[2];

        System.out.println(String.format("name %s xPart %s yPart %s", name, xPart, yPart));

        int x = Integer.parseInt(xPart);
        int y = Integer.parseInt(yPart);

        return new Vec2i(x, y);
    }

    private static class CloseListener implements KeyListener {
        private final GuiThread thread;

        public CloseListener(GuiThread thread) {
            this.thread = thread;
        }

        public void keyPressed(KeyEvent keyEvent) {
            Point mousePoint = MouseInfo.getPointerInfo().getLocation();
            Component comp = thread.frame.getContentPane().getComponentAt(mousePoint);

            JButton gridButton = null;
            if (comp instanceof JButton) {
                gridButton = (JButton) comp;
            }

            int keyCode = keyEvent.getKeyCode();

            if (keyCode == KeyEvent.VK_ESCAPE) {
                Board.ledClear(true);
                if (keyEvent.isShiftDown()) {
                    thread.frame.dispose();
                    Main.running = false;
                }
            } else if (keyCode >= KeyEvent.VK_0 && keyCode <= KeyEvent.VK_9 && gridButton != null) {
                String buttonName = gridButton.getName();
                Vec2i gridPoint = thread.parseGridName(buttonName);
                if (gridPoint == null) {
                    System.out.println(String.format("%s is not a grid button", buttonName));
                    return;
                }

                // Because VK_0 - VK_9 is sequential, subtracting will get us the number typed
                int colorIndex = keyCode - KeyEvent.VK_0;

                Board.ledOn(gridPoint.x, gridPoint.y, colorIndex);
            } else if (keyCode == KeyEvent.VK_A && gridButton != null) {
                Vec2i gridPoint = thread.parseGridName(gridButton.getName());
                if (gridPoint == null) {
                    System.out.println(String.format("Missing gridPoint for %s", gridButton.getName()));
                } else {
                    if (thread.selectedGridCoord != null) {
                        thread.selectedSquares.add(gridPoint);
                        System.out.println(String.format("Adding %d, %d", gridPoint.x, gridPoint.y));
                    } else {
                        System.out.println("Skipping adding button as selectedGridCoord is null");
                    }
                }
            } else if (keyCode == KeyEvent.VK_C) {
                thread.selectedSquares.clear();
                thread.frame.repaint();
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

    private class ButtonClickListener implements MouseListener {
        private final int index;

        public ButtonClickListener(int index) {
            this.index = index;
        }

        public void mouseClicked(MouseEvent mouseEvent) {
            System.out.println(String.format("Select button index %d", index));

            int mx = mouseEvent.getX();
            int my = mouseEvent.getY();

            Component comp = (Component) mouseEvent.getSource(); // frame.getContentPane().getComponentAt(mx, my);
            if (comp == null) {
                System.out.println("GuiThread> No source in MouseEvent");
                return;
            }
            if (!(comp instanceof JButton)) {
                System.out.println("GuiThread> Source is not a JButton");
                return;
            }

            String buttonName = comp.getName();
            Vec2i gridCoords = parseGridName(buttonName);
            if (gridCoords == null) {
                System.out.println(
                    String.format("Failed to parse name of JButton %s", buttonName)
                );
                return;
            }

            selectedGridCoord = gridCoords;

            if (mouseEvent.getButton() == MouseEvent.BUTTON1) {
                Board.ledClear(true);

                String text = ((JButton) comp).getText();
                int index = Integer.parseInt(text);

                int distance = -1;

                for (int i = 0; i < Main.config.size(); ++i){
                    ConfigEntry entry = Main.config.get(i);
                    if (entry.value == index) {
                        distance = entry.distance;
                        break;
                    }
                }

                if (distance > -1) {
                    for (int i = 0; i < 8; ++i) {
                        for (int j = 0; j < 8; ++j) {
                            Vec2i gridPoint = new Vec2i(i, j);

                            double dist = BoardUtil.distance(gridPoint, gridCoords);
                            if (dist <= distance) {
                                Board.ledOn(i, j, 1);
                                System.out.printf("Setting (%d, %d) on as dist = %.2f %n", i, j, dist);
                            }
                        }
                    }
                } else {
                    System.out.printf("Failed to find distance for %d %n", index);
                }

                // Queued after all others have completed
                Board.ledClear(true);
            } else if (mouseEvent.getButton() == MouseEvent.BUTTON3) {
                contextMenu.show(frame, comp.getX() + mx, comp.getY() + my);
                System.out.println(String.format("Showing context menu at (%d, %d)", mx, my));
            }
        }

        public void mousePressed(MouseEvent mouseEvent) { }
        public void mouseReleased(MouseEvent mouseEvent) { }
        public void mouseEntered(MouseEvent mouseEvent) { }
        public void mouseExited(MouseEvent mouseEvent) { }
    }
}
