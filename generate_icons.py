"""
Generate minimalistic line-art icons for the espresso machine UI.
White strokes on transparent background (RGBA PNG).
Matches the aesthetic from the HTML preview.
"""
from PIL import Image, ImageDraw
import math
import os

OUT_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "images")
os.makedirs(OUT_DIR, exist_ok=True)

# Colors (white on transparent, icons will be recolored via LVGL styles)
W = (255, 255, 255, 255)
W_DIM = (255, 255, 255, 140)
W_FAINT = (255, 255, 255, 80)
TRANSPARENT = (0, 0, 0, 0)


def save(img, name):
    path = os.path.join(OUT_DIR, name)
    img.save(path)
    print(f"  {name}  ({img.size[0]}x{img.size[1]})")


# ─────────────────────────────────────────────
# Coffee cup  (48x48)
# ─────────────────────────────────────────────
def draw_coffee():
    S = 48
    img = Image.new("RGBA", (S, S), TRANSPARENT)
    d = ImageDraw.Draw(img)
    sw = 2  # stroke width

    # Cup body (rounded rect)
    d.rounded_rectangle([8, 20, 32, 42], radius=4, outline=W, width=sw)

    # Handle (arc on right)
    d.arc([30, 24, 42, 38], start=-60, end=60, fill=W, width=sw)

    # Steam lines (3 gentle sine curves)
    for cx, phase in [(14, 0), (21, 1.2), (28, 0.6)]:
        pts = []
        for i in range(14):
            t = i / 13.0
            y = 17 - t * 14
            x = cx + math.sin(t * math.pi * 2 + phase) * 2.2
            pts.append((x, y))
        # Fade out at top
        alpha = int(255 * (1 - 0)) if True else 140
        d.line(pts, fill=W_DIM, width=sw, joint="curve")

    save(img, "icon_coffee.png")


# ─────────────────────────────────────────────
# Steam / vapor  (48x48)
# ─────────────────────────────────────────────
def draw_steam():
    S = 48
    img = Image.new("RGBA", (S, S), TRANSPARENT)
    d = ImageDraw.Draw(img)
    sw = 2

    # Base platform
    d.rounded_rectangle([8, 38, 40, 44], radius=2, outline=W, width=sw)

    # Three wavy steam columns
    for cx, phase in [(16, 0.0), (24, 1.5), (32, 0.8)]:
        pts = []
        for i in range(18):
            t = i / 17.0
            y = 35 - t * 30
            x = cx + math.sin(t * math.pi * 2.5 + phase) * 3.5
            pts.append((x, y))
        d.line(pts, fill=W, width=sw, joint="curve")

    save(img, "icon_steam.png")


# ─────────────────────────────────────────────
# Water droplet  (48x48)
# ─────────────────────────────────────────────
def draw_water():
    S = 48
    img = Image.new("RGBA", (S, S), TRANSPARENT)
    d = ImageDraw.Draw(img)
    sw = 2

    cx = 24
    tip_y = 6
    circle_cy = 30
    r = 13

    # Build droplet as smooth polygon
    pts = [(cx, tip_y)]

    # Right side: bezier-like from tip curving into circle
    steps = 40
    for i in range(1, steps + 1):
        t = i / steps
        # Angle along circle from top (-90°) to right (0°) to bottom (90°)
        angle = -90 + t * 180
        rad = math.radians(angle)

        # x follows the circle, y blends from tip to circle
        px = cx + r * math.cos(rad)
        py = circle_cy + r * math.sin(rad)

        # For the top portion, narrow the x toward the tip
        if t < 0.4:
            blend = t / 0.4
            blend = blend * blend  # ease-in
            px = cx + (px - cx) * blend
            py = tip_y + (py - tip_y) * blend

        pts.append((px, py))

    # Left side: mirror (bottom to top)
    for i in range(steps, 0, -1):
        t = i / steps
        angle = -90 + t * 180
        rad = math.radians(angle)

        px = cx - r * math.cos(rad)
        py = circle_cy + r * math.sin(rad)

        if t < 0.4:
            blend = t / 0.4
            blend = blend * blend
            px = cx + (px - cx) * blend
            py = tip_y + (py - tip_y) * blend

        pts.append((px, py))

    d.polygon(pts, outline=W, fill=None)
    d.line(pts + [pts[0]], fill=W, width=sw, joint="curve")

    save(img, "icon_water.png")


# ─────────────────────────────────────────────
# Settings gear  (48x48)
# ─────────────────────────────────────────────
def draw_settings():
    S = 48
    img = Image.new("RGBA", (S, S), TRANSPARENT)
    d = ImageDraw.Draw(img)
    sw = 2

    cx, cy = 24, 24
    r_hole = 6    # center hole
    r_body = 13   # gear body
    r_tooth = 18  # tooth tip
    teeth = 8
    tooth_half_w = math.pi / teeth * 0.55  # half angular width of tooth

    # Build gear outline
    pts = []
    steps_per_segment = 4
    for i in range(teeth):
        base_angle = i * 2 * math.pi / teeth - math.pi / 2

        # Valley (between teeth)
        for s in range(steps_per_segment):
            t = s / steps_per_segment
            a = base_angle - tooth_half_w + t * 0 - (math.pi / teeth - tooth_half_w)
            # simplified: just place points at valleys and peaks
        # Valley start
        a1 = base_angle - tooth_half_w
        a0 = base_angle - math.pi / teeth
        # Points: valley, rise, peak, fall
        pts.append((cx + r_body * math.cos(a0), cy + r_body * math.sin(a0)))
        pts.append((cx + r_body * math.cos(a1), cy + r_body * math.sin(a1)))
        a2 = base_angle + tooth_half_w
        pts.append((cx + r_tooth * math.cos(a1), cy + r_tooth * math.sin(a1)))
        pts.append((cx + r_tooth * math.cos(a2), cy + r_tooth * math.sin(a2)))
        pts.append((cx + r_body * math.cos(a2), cy + r_body * math.sin(a2)))

    pts.append(pts[0])
    d.line(pts, fill=W, width=sw, joint="curve")

    # Center circle (hole)
    d.ellipse(
        [cx - r_hole, cy - r_hole, cx + r_hole, cy + r_hole],
        outline=W, width=sw
    )

    save(img, "icon_settings.png")


# ─────────────────────────────────────────────
# Back chevron  (32x32)
# ─────────────────────────────────────────────
def draw_back():
    S = 32
    img = Image.new("RGBA", (S, S), TRANSPARENT)
    d = ImageDraw.Draw(img)

    # Clean chevron-left
    pts = [(20, 6), (10, 16), (20, 26)]
    d.line(pts, fill=W, width=2, joint="curve")

    save(img, "icon_back.png")


# ─────────────────────────────────────────────
# Play triangle  (32x32)
# ─────────────────────────────────────────────
def draw_play():
    S = 32
    img = Image.new("RGBA", (S, S), TRANSPARENT)
    d = ImageDraw.Draw(img)

    # Solid filled triangle pointing right
    pts = [(11, 6), (25, 16), (11, 26)]
    d.polygon(pts, fill=W)

    save(img, "icon_play.png")


# ─────────────────────────────────────────────
# Pause bars  (32x32)
# ─────────────────────────────────────────────
def draw_pause():
    S = 32
    img = Image.new("RGBA", (S, S), TRANSPARENT)
    d = ImageDraw.Draw(img)

    # Two vertical bars
    d.rounded_rectangle([9, 7, 14, 25], radius=1, fill=W)
    d.rounded_rectangle([18, 7, 23, 25], radius=1, fill=W)

    save(img, "icon_pause.png")


if __name__ == "__main__":
    print("Generating icons...")
    draw_coffee()
    draw_steam()
    draw_water()
    draw_settings()
    draw_back()
    draw_play()
    draw_pause()
    print("Done!")
