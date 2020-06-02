package me.hdt;

import java.util.Objects;

public class Vec2d {

    public Vec2d() {
        this.x = 0d;
        this.y = 0d;
    }

    public Vec2d(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public Vec2d(int x, int y) {
        this.x = (double) x;
        this.y = (double) y;
    }

    public double x;

    public double y;

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public void setX(double x) {
        this.x = x;
    }

    public void setY(double y) {
        this.y = y;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Vec2d vec2d = (Vec2d) o;
        return Double.compare(vec2d.x, x) == 0 &&
                Double.compare(vec2d.y, y) == 0;
    }

    @Override
    public int hashCode() {
        return Objects.hash(x, y);
    }
}
