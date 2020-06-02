package me.hdt.util;

import me.hdt.Vec2d;
import me.hdt.Vec2i;

import java.awt.*;

public class BoardUtil {

    public static double distance(Vec2d p1, Vec2d p2) {
        double dx = p2.getX() - p1.getX();
        double dy = p2.getY() - p1.getY();

        return Math.sqrt(Math.pow(dx, 2) + Math.pow(dy, 2));
    }

    public static double distance(Vec2i p1, Vec2i p2) {
        double dx = p2.getX() - p1.getX();
        double dy = p2.getY() - p1.getY();

        return Math.sqrt(Math.pow(dx, 2) + Math.pow(dy, 2));
    }

}
