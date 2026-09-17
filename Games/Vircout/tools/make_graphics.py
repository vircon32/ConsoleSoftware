from PIL import Image, ImageDraw, ImageFont
import os, math, random

W,H=640,360
ASSET=os.path.join(os.path.dirname(__file__),'..','assets')
os.makedirs(ASSET,exist_ok=True)

def load_font(size, candidates):
    """Load a similar font at the requested size on Linux, Windows or macOS.

    The old generator used Linux-only absolute paths. On other systems that
    made all title fonts fall back to Pillow's tiny default bitmap font.
    """
    for candidate in candidates:
        try:
            return ImageFont.truetype(candidate, size)
        except OSError:
            pass

    # Recent Pillow versions provide a scalable built-in fallback. This keeps
    # the requested point size even on an unusual system with none of the
    # fonts above installed.
    try:
        return ImageFont.load_default(size=size)
    except TypeError:
        return ImageFont.load_default()

FONT_BIG=load_font(72, [
    '/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed-BoldOblique.ttf',
    'DejaVuSansCondensed-BoldOblique.ttf',
    r'C:\\Windows\\Fonts\\arialbi.ttf',
    '/System/Library/Fonts/Supplemental/Arial Bold Italic.ttf',
])
FONT_MED=load_font(22, [
    '/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed-Bold.ttf',
    'DejaVuSansCondensed-Bold.ttf',
    r'C:\\Windows\\Fonts\\arialbd.ttf',
    '/System/Library/Fonts/Supplemental/Arial Bold.ttf',
])
FONT_SMALL=load_font(13, [
    '/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf',
    'DejaVuSansMono.ttf',
    r'C:\\Windows\\Fonts\\consola.ttf',
    '/System/Library/Fonts/Menlo.ttc',
])

def vertical_gradient(top,bottom):
    im=Image.new('RGB',(W,H))
    p=im.load()
    for y in range(H):
        t=y/(H-1)
        c=tuple(round(top[i]*(1-t)+bottom[i]*t) for i in range(3))
        for x in range(W): p[x,y]=c
    return im

def stars(draw,seed,count,area=(0,0,W,H), palette=((75,130,190),(115,190,230),(220,245,255))):
    rng=random.Random(seed)
    x0,y0,x1,y1=area
    for _ in range(count):
        x=rng.randrange(x0,x1); y=rng.randrange(y0,y1)
        c=rng.choice(palette)
        r=1 if rng.random()<0.88 else 2
        draw.rectangle((x,y,x+r-1,y+r-1),fill=c)

def centered(draw,text,y,font,fill,stroke=0,stroke_fill=None):
    box=draw.textbbox((0,0),text,font=font,stroke_width=stroke)
    x=(W-(box[2]-box[0]))//2
    draw.text((x,y),text,font=font,fill=fill,stroke_width=stroke,stroke_fill=stroke_fill)

# Title ----------------------------------------------------------------------
im=vertical_gradient((5,8,30),(11,20,48)); d=ImageDraw.Draw(im)
stars(d,101,95,(0,0,W,280))
# perspective neon floor
horizon=258
for y in range(horizon,H,12):
    alpha=(y-horizon)/(H-horizon)
    col=(14+int(16*alpha),36+int(35*alpha),68+int(55*alpha))
    d.line((0,y,W,y),fill=col,width=1)
for x in range(-160,801,48):
    d.line((W//2,horizon,x,H),fill=(18,49,86),width=1)
# glow bands behind logo
for off,col in [(8,(8,31,65)),(5,(10,53,91)),(2,(12,87,125))]:
    d.rounded_rectangle((82-off,68-off,558+off,190+off),radius=18,outline=col,width=3)
# logo shadow and foreground
centered(d,'LUA POWERED',47,FONT_MED,(120,220,255),1,(0,50,80))
centered(d,'VIRCOUT',78,FONT_BIG,(35,220,255),3,(0,55,100))
centered(d,'VIRCOUT',74,FONT_BIG,(240,250,255),1,(35,220,255))
centered(d,'NEON BRICK ARENA',176,FONT_MED,(255,174,64),1,(90,35,0))
# tiny decorative bricks
colors=[(35,220,255),(255,146,39),(236,74,42),(170,200,215)]
for row in range(2):
    for col in range(9):
        x=126+col*44 + (22 if row else 0); y=214+row*20
        d.rounded_rectangle((x,y,x+34,y+10),radius=2,fill=colors[(col+row)%len(colors)],outline=(240,250,255))
im.save(os.path.join(ASSET,'title.png'))

# Gameplay -------------------------------------------------------------------
im=vertical_gradient((6,12,25),(8,20,35)); d=ImageDraw.Draw(im)
# inner play field subtle gradient
for y in range(36,H):
    t=(y-36)/(H-36)
    c=(7+int(3*t),18+int(8*t),31+int(13*t))
    d.line((32,y,603,y),fill=c)
# sparse stars / dust in playable area
stars(d,222,70,(34,40,602,325),palette=((20,55,78),(24,74,94),(38,94,110)))
# subtle vertical circuit lines
for x in range(64,604,72):
    d.line((x,42,x,324),fill=(9,31,48))
    for y in range(76,316,80):
        d.rectangle((x-2,y-2,x+2,y+2),fill=(14,52,68))
# top HUD rail
for y,col in [(0,(7,10,18)),(3,(17,39,55)),(30,(10,50,69)),(34,(45,199,229)),(35,(6,78,104))]:
    d.rectangle((0,y,W-1,y if y not in (0,) else 2),fill=col)
d.rectangle((0,4,W-1,29),fill=(8,16,29))
# HUD panels
for x0,x1 in [(30,180),(450,610)]:
    d.rounded_rectangle((x0,6,x1,28),radius=5,fill=(9,28,43),outline=(24,96,122),width=1)
# side rails exactly outside playfield
for x0,x1,inner in [(0,31,31),(604,639,604)]:
    d.rectangle((x0,36,x1,H-1),fill=(5,10,18))
    # layered metallic/cyan edge
    if inner==31:
        d.rectangle((20,36,31,H-1),fill=(8,35,49))
        d.rectangle((27,36,31,H-1),fill=(18,83,104))
        d.line((31,36,31,H-1),fill=(52,218,238),width=1)
        for y in range(52,H,42): d.rectangle((8,y,18,y+3),fill=(14,53,67))
    else:
        d.rectangle((604,36,615,H-1),fill=(8,35,49))
        d.rectangle((604,36,608,H-1),fill=(18,83,104))
        d.line((604,36,604,H-1),fill=(52,218,238),width=1)
        for y in range(52,H,42): d.rectangle((621,y,631,y+3),fill=(14,53,67))
# top corners / bolts
for x in (10,21,619,630):
    d.ellipse((x-2,17,x+2,21),fill=(89,136,150))
im.save(os.path.join(ASSET,'gameplay.png'))

# Ending ---------------------------------------------------------------------
im=vertical_gradient((3,5,18),(12,20,46)); d=ImageDraw.Draw(im)
stars(d,404,125,(0,0,W,300),palette=((90,125,190),(120,190,235),(235,245,255),(255,194,105)))
# distant planet / horizon
for r,c in [(130,(17,40,70)),(118,(23,55,91)),(104,(31,72,112))]:
    d.ellipse((W//2-r,250-r//3,W//2+r,250+r*2-r//3),fill=c)
# mask lower half to create horizon arc
d.rectangle((0,278,W,H),fill=(5,9,23))
d.line((0,278,W,278),fill=(58,194,225),width=2)
# city silhouettes / antenna dots
rng=random.Random(44)
x=0
while x<W:
    bw=rng.randint(12,34); bh=rng.randint(6,25)
    d.rectangle((x,278-bh,x+bw,278),fill=(4,9,18))
    if rng.random()<0.25: d.rectangle((x+bw//2,270-bh,x+bw//2+1,278-bh),fill=(30,80,100))
    x += bw+rng.randint(2,6)
# understated ending frame
d.rounded_rectangle((118,80,522,245),radius=15,outline=(18,63,96),width=2)
d.rounded_rectangle((124,86,516,239),radius=12,outline=(12,33,59),width=1)
im.save(os.path.join(ASSET,'ending.png'))



# Sprite atlas retouch --------------------------------------------------------
# Keep collision regions exactly 44x16. These changes only make the hard and
# unbreakable artwork occupy the same visual footprint as the normal block.
def draw_block_shell(draw, ox, oy, outer, inner, highlight, damaged=False, metal=False):
    # Transparent rounded 44x16 silhouette, matching the normal block footprint.
    # Corners use the same 2/1/0 px staircase as the normal cyan sprite.
    # Outer shell
    draw.rectangle((ox+2,oy,ox+41,oy), fill=outer)
    draw.rectangle((ox+1,oy+1,ox+42,oy+1), fill=outer)
    draw.rectangle((ox,oy+2,ox+43,oy+13), fill=outer)
    draw.rectangle((ox+1,oy+14,ox+42,oy+14), fill=outer)
    draw.rectangle((ox+2,oy+15,ox+41,oy+15), fill=outer)
    # Body
    draw.rectangle((ox+2,oy+2,ox+41,oy+13), fill=inner)
    draw.rectangle((ox+3,oy+3,ox+40,oy+12), fill=inner)
    # Inner bevel / inset panel
    draw.rectangle((ox+5,oy+4,ox+38,oy+11), outline=highlight, width=1)
    draw.line((ox+6,oy+5,ox+37,oy+5), fill=highlight)
    if metal:
        # Rivets and a cool steel center.
        draw.rectangle((ox+7,oy+6,ox+36,oy+9), fill=(88,105,125,255))
        draw.point((ox+6,oy+7), fill=(220,232,242,255))
        draw.point((ox+37,oy+7), fill=(220,232,242,255))
    if damaged:
        # Bright crack that remains readable at native resolution.
        crack=(255,225,125,255)
        pts=[(ox+23,oy+3),(ox+20,oy+6),(ox+24,oy+8),(ox+19,oy+12)]
        for a,b in zip(pts,pts[1:]): draw.line((*a,*b), fill=crack, width=1)
        draw.line((ox+24,oy+8,ox+29,oy+5), fill=crack, width=1)


def patch_sprite_atlas():
    path=os.path.join(ASSET,'sprites.png')
    atlas=Image.open(path).convert('RGBA')
    d=ImageDraw.Draw(atlas)
    # Clear only the three block cells before repainting them.
    for box in [(146,0,189,15),(192,0,235,15),(238,0,281,15)]:
        d.rectangle(box, fill=(0,0,0,0))
    draw_block_shell(d,146,0,(255,181,63,255),(226,104,26,255),(255,224,128,255))
    draw_block_shell(d,192,0,(255,155,55,255),(183,68,28,255),(255,195,91,255),damaged=True)
    draw_block_shell(d,238,0,(220,232,242,255),(63,76,94,255),(185,204,220,255),metal=True)

    # Enlarge the unbreakable flash inside its existing 56x28 animation canvas.
    # Four frames: quick full-block white/cyan flash that fades back to the metal tone.
    metal_frames=[
        ((235,250,255,255),(155,225,245,255)),
        ((205,240,255,255),(105,190,225,255)),
        ((150,210,235,255),(75,145,180,255)),
        ((105,155,185,220),(55,95,120,220)),
    ]
    starts=[0,58,116,174]
    for sx,(edge,body) in zip(starts,metal_frames):
        d.rectangle((sx,110,sx+55,137), fill=(0,0,0,0))
        # 48x20 flash centered in the 56x28 canvas, visibly larger than the block.
        x0=sx+4; y0=114; x1=sx+51; y1=133
        d.rectangle((x0+2,y0,x1-2,y1), fill=edge)
        d.rectangle((x0,y0+2,x1,y1-2), fill=edge)
        d.rectangle((x0+2,y0+2,x1-2,y1-2), fill=body)
        d.rectangle((x0+5,y0+5,x1-5,y1-5), outline=(245,255,255,255), width=1)
    atlas.save(path)

patch_sprite_atlas()

print('Generated title.png, gameplay.png and ending.png')


# --- Override patch after art direction pass --------------------------------
def patch_sprite_atlas_art_direction_pass():
    from PIL import Image, ImageDraw
    path=os.path.join(ASSET,'sprites.png')
    atlas=Image.open(path).convert('RGBA')
    d=ImageDraw.Draw(atlas)

    def rounded_rect_stair(draw, x,y,w,h, outer, inner, radius_style='rounded'):
        if radius_style=='rounded':
            draw.rectangle((x+2,y,x+w-3,y), fill=outer)
            draw.rectangle((x+1,y+1,x+w-2,y+1), fill=outer)
            draw.rectangle((x,y+2,x+w-1,y+h-3), fill=outer)
            draw.rectangle((x+1,y+h-2,x+w-2,y+h-2), fill=outer)
            draw.rectangle((x+2,y+h-1,x+w-3,y+h-1), fill=outer)
            draw.rectangle((x+2,y+2,x+w-3,y+h-3), fill=inner)
            draw.rectangle((x+3,y+3,x+w-4,y+h-4), fill=inner)
        else:
            draw.rectangle((x,y,x+w-1,y+h-1), fill=outer)
            draw.rectangle((x+1,y+1,x+w-2,y+h-2), fill=inner)
            draw.rectangle((x+2,y+2,x+w-3,y+h-3), fill=inner)

    def draw_hard_normal(draw,x,y):
        outer=(255,187,72,255); inner=(214,102,22,255)
        rounded_rect_stair(draw,x,y,44,16,outer,inner,'rounded')
        draw.line((x+4,y+3,x+39,y+3), fill=(255,223,126,255))
        draw.line((x+4,y+12,x+39,y+12), fill=(166,64,15,255))
        c1=(255,170,70,255); c2=(236,125,33,255)
        panel=(x+5,y+4,x+38,y+11)
        for sx in range(panel[0]-8, panel[2]+8, 8):
            draw.polygon([(sx,panel[3]),(sx+4,panel[3]),(sx+12,panel[1]),(sx+8,panel[1])], fill=c1)
        for sx in range(panel[0]-4, panel[2]+12, 8):
            draw.polygon([(sx,panel[3]),(sx+4,panel[3]),(sx+12,panel[1]),(sx+8,panel[1])], fill=c2)
        draw.rectangle((x+5,y+4,x+38,y+11), outline=(255,232,150,255), width=1)
        draw.line((x+6,y+5,x+37,y+5), fill=(255,245,190,255))

    def draw_hard_damaged(draw,x,y):
        draw_hard_normal(draw,x,y)
        crack=(255,240,195,255); shadow=(90,35,10,255)
        dark_lines=[((x+8,y+4),(x+13,y+6)),((x+13,y+6),(x+18,y+5)),((x+18,y+5),(x+22,y+8)),((x+22,y+8),(x+27,y+7)),((x+27,y+7),(x+32,y+10)),((x+32,y+10),(x+36,y+9)),((x+14,y+11),(x+18,y+8)),((x+18,y+8),(x+21,y+11)),((x+24,y+4),(x+24,y+8)),((x+30,y+6),(x+30,y+12)),((x+10,y+9),(x+8,y+12)),((x+35,y+4),(x+38,y+6))]
        light_lines=[((x+8,y+3),(x+13,y+5)),((x+13,y+5),(x+18,y+4)),((x+18,y+4),(x+22,y+7)),((x+22,y+7),(x+27,y+6)),((x+27,y+6),(x+32,y+9)),((x+32,y+9),(x+36,y+8)),((x+14,y+10),(x+18,y+7)),((x+18,y+7),(x+21,y+10)),((x+24,y+3),(x+24,y+7)),((x+30,y+5),(x+30,y+11)),((x+10,y+8),(x+8,y+11)),((x+35,y+3),(x+38,y+5))]
        for a,b in dark_lines: draw.line((*a,*b), fill=shadow)
        for a,b in light_lines: draw.line((*a,*b), fill=crack)

    def draw_unbreakable(draw,x,y):
        outer=(198,209,220,255); inner=(73,84,98,255); dark=(48,57,70,255)
        rounded_rect_stair(draw,x,y,44,16,outer,inner,'square')
        draw.rectangle((x+3,y+3,x+40,y+12), fill=(88,102,118,255))
        draw.rectangle((x+5,y+4,x+38,y+11), outline=(165,180,196,255), width=1)
        draw.rectangle((x+16,y+4,x+27,y+11), fill=(108,123,139,255), outline=(185,198,210,255), width=1)
        draw.line((x+6,y+6,x+15,y+6), fill=dark)
        draw.line((x+28,y+6,x+37,y+6), fill=dark)
        draw.line((x+6,y+9,x+15,y+9), fill=(130,145,160,255))
        draw.line((x+28,y+9,x+37,y+9), fill=(130,145,160,255))
        for bx,by in [(6,5),(36,5),(6,10),(36,10)]:
            draw.rectangle((x+bx,y+by,x+bx+1,y+by+1), fill=(220,230,240,255))
            draw.point((x+bx+1,y+by+1), fill=(120,135,150,255))
        draw.line((x+1,y+1,x+42,y+1), fill=(230,240,248,255))
        draw.line((x+1,y+14,x+42,y+14), fill=(36,46,58,255))

    for box in [(146,0,189,15),(192,0,235,15),(238,0,281,15)]:
        d.rectangle(box, fill=(0,0,0,0))
    draw_hard_normal(d,146,0)
    draw_hard_damaged(d,192,0)
    draw_unbreakable(d,238,0)

    starts=[0,58,116,174]
    centers=[12,22,34,44]
    for sx,cx in zip(starts,centers):
        d.rectangle((sx,50,sx+55,77), fill=(0,0,0,0))
        bx=sx+6; by=56
        rounded_rect_stair(d,bx,by,44,16,(255,165,60,90),(186,84,22,70),'rounded')
        for offset,color in [(-4,(255,230,120,80)),(-2,(255,235,150,140)),(0,(255,250,220,255)),(2,(255,235,150,140)),(4,(255,220,100,70))]:
            x=cx+offset+sx
            d.rectangle((x,54,x+1,72), fill=color)
        d.line((sx+8,64,sx+48,64), fill=(255,214,110,110))
        for px,py in [(cx+sx-6,59),(cx+sx+6,60),(cx+sx-2,68),(cx+sx+9,67)]:
            d.point((px,py), fill=(255,245,200,220))

    atlas.save(path)

patch_sprite_atlas_art_direction_pass()


# --- Final polish pass: hard/unbreakable block differentiation ---------
def patch_sprite_atlas_final_polish():
    from PIL import Image, ImageDraw
    path=os.path.join(ASSET,'sprites.png')
    atlas=Image.open(path).convert('RGBA')
    d=ImageDraw.Draw(atlas)
    def rounded_shell(draw,x,y,w,h, outer, inner):
        draw.rectangle((x+2,y,x+w-3,y), fill=outer); draw.rectangle((x+1,y+1,x+w-2,y+1), fill=outer); draw.rectangle((x,y+2,x+w-1,y+h-3), fill=outer); draw.rectangle((x+1,y+h-2,x+w-2,y+h-2), fill=outer); draw.rectangle((x+2,y+h-1,x+w-3,y+h-1), fill=outer); draw.rectangle((x+2,y+2,x+w-3,y+h-3), fill=inner); draw.rectangle((x+3,y+3,x+w-4,y+h-4), fill=inner)
    def square_shell(draw,x,y,w,h, outer, inner):
        draw.rectangle((x,y,x+w-1,y+h-1), fill=outer); draw.rectangle((x+1,y+1,x+w-2,y+h-2), fill=inner); draw.rectangle((x+2,y+2,x+w-3,y+h-3), fill=inner)
    def draw_hard(draw,x,y,damaged=False):
        outer=(255,188,78,255); inner=(212,99,22,255); rounded_shell(draw,x,y,44,16,outer,inner)
        panel=(x+5,y+4,x+38,y+11); c1=(255,178,78,255); c2=(233,127,33,255)
        for sx in range(panel[0]-12,panel[2]+12,10): draw.polygon([(sx,panel[3]),(sx+5,panel[3]),(sx+13,panel[1]),(sx+8,panel[1])], fill=c1)
        for sx in range(panel[0]-7,panel[2]+17,10): draw.polygon([(sx,panel[3]),(sx+5,panel[3]),(sx+13,panel[1]),(sx+8,panel[1])], fill=c2)
        draw.rectangle(panel, outline=(255,232,150,255), width=1); draw.line((x+5,y+3,x+38,y+3), fill=(255,238,180,255)); draw.line((x+5,y+12,x+38,y+12), fill=(158,62,15,255))
        if damaged:
            light=(255,244,210,255); shadow=(96,42,10,255); crack_pairs=[((x+4,y+4),(x+9,y+6)),((x+9,y+6),(x+14,y+5)),((x+14,y+5),(x+18,y+8)),((x+18,y+8),(x+23,y+7)),((x+23,y+7),(x+28,y+10)),((x+28,y+10),(x+34,y+8)),((x+34,y+8),(x+40,y+10)),((x+8,y+12),(x+12,y+9)),((x+12,y+9),(x+16,y+12)),((x+21,y+3),(x+21,y+8)),((x+27,y+4),(x+25,y+7)),((x+31,y+5),(x+31,y+12)),((x+36,y+3),(x+39,y+6)),((x+5,y+9),(x+3,y+12))]
            for a,b in crack_pairs: draw.line((a[0],a[1]+1,b[0],b[1]+1), fill=shadow)
            for a,b in crack_pairs: draw.line((a[0],a[1],b[0],b[1]), fill=light)
    def draw_unbreakable(draw,x,y):
        outer=(216,224,232,255); inner=(74,84,98,255); square_shell(draw,x,y,44,16,outer,inner); draw.line((x+1,y+1,x+42,y+1), fill=(242,247,252,255)); draw.line((x+1,y+14,x+42,y+14), fill=(44,52,63,255)); draw.rectangle((x+3,y+3,x+40,y+12), fill=(92,104,120,255)); draw.rectangle((x+5,y+4,x+38,y+11), outline=(168,182,196,255), width=1); draw.rectangle((x+15,y+4,x+28,y+11), fill=(114,126,141,255), outline=(196,206,216,255), width=1);
        for bx,by in [(5,5),(37,5),(5,10),(37,10)]: draw.rectangle((x+bx,y+by,x+bx+1,y+by+1), fill=(238,244,248,255)); draw.point((x+bx+1,y+by+1), fill=(126,138,150,255))
        draw.line((x+7,y+6,x+13,y+6), fill=(52,60,73,255)); draw.line((x+7,y+9,x+13,y+9), fill=(132,144,156,255)); draw.line((x+30,y+6,x+36,y+6), fill=(52,60,73,255)); draw.line((x+30,y+9,x+36,y+9), fill=(132,144,156,255))
    for box in [(146,0,189,15),(192,0,235,15),(238,0,281,15)]: d.rectangle(box, fill=(0,0,0,0))
    draw_hard(d,146,0,False); draw_hard(d,192,0,True); draw_unbreakable(d,238,0)
    starts=[0,58,116,174]; shine_x=[6,16,26,36]
    for sx,cx in zip(starts,shine_x):
        d.rectangle((sx,110,sx+55,137), fill=(0,0,0,0)); base_x=sx+6; base_y=116
        square_shell(d,base_x,base_y,44,16,(190,205,220,70),(78,92,108,60)); d.rectangle((base_x+3,base_y+3,base_x+40,base_y+12), fill=(94,108,124,60))
        for off,col in [(-5,(120,210,255,60)),(-3,(170,230,255,100)),(-1,(240,250,255,210)),(1,(170,230,255,100)),(3,(120,210,255,60))]:
            x=base_x+cx+off; d.rectangle((x,114,x+1,134), fill=col)
        d.line((base_x+4,124,base_x+39,124), fill=(190,235,255,80)); d.rectangle((base_x+5,118,base_x+38,131), outline=(220,245,255,70), width=1)
    atlas.save(path)

patch_sprite_atlas_final_polish()


# --- Final polish pass 2: restore selected FX and refine block art ----
def patch_sprite_atlas_final_polish_2():
    from PIL import Image, ImageDraw
    path=os.path.join(ASSET,'sprites.png')
    atlas=Image.open(path).convert('RGBA')
    d=ImageDraw.Draw(atlas)
    def rounded_shell(draw,x,y,w,h, outer, inner):
        draw.rectangle((x+2,y,x+w-3,y), fill=outer); draw.rectangle((x+1,y+1,x+w-2,y+1), fill=outer); draw.rectangle((x,y+2,x+w-1,y+h-3), fill=outer); draw.rectangle((x+1,y+h-2,x+w-2,y+h-2), fill=outer); draw.rectangle((x+2,y+h-1,x+w-3,y+h-1), fill=outer); draw.rectangle((x+2,y+2,x+w-3,y+h-3), fill=inner); draw.rectangle((x+3,y+3,x+w-4,y+h-4), fill=inner)
    def square_shell(draw,x,y,w,h, outer, inner):
        draw.rectangle((x,y,x+w-1,y+h-1), fill=outer); draw.rectangle((x+1,y+1,x+w-2,y+h-2), fill=inner); draw.rectangle((x+2,y+2,x+w-3,y+h-3), fill=inner)
    def draw_hard_damaged(draw,x,y):
        outer=(234,151,57,255); inner=(168,73,18,255); c1=(228,144,58,255); c2=(196,100,28,255); hi=(255,223,150,255); lo=(122,47,12,255)
        rounded_shell(draw,x,y,44,16,outer,inner); panel=(x+5,y+4,x+38,y+11)
        for sx in range(panel[0]-12,panel[2]+12,10): draw.polygon([(sx,panel[3]),(sx+5,panel[3]),(sx+13,panel[1]),(sx+8,panel[1])], fill=c1)
        for sx in range(panel[0]-7,panel[2]+17,10): draw.polygon([(sx,panel[3]),(sx+5,panel[3]),(sx+13,panel[1]),(sx+8,panel[1])], fill=c2)
        draw.rectangle(panel, outline=(255,232,150,255), width=1); draw.line((x+5,y+3,x+38,y+3), fill=hi); draw.line((x+5,y+12,x+38,y+12), fill=lo)
        light=(255,244,210,255); shadow=(92,35,9,255); segs=[((x+2,y+4),(x+8,y+6)),((x+8,y+6),(x+14,y+4)),((x+14,y+4),(x+18,y+8)),((x+18,y+8),(x+24,y+6)),((x+24,y+6),(x+30,y+10)),((x+30,y+10),(x+37,y+7)),((x+37,y+7),(x+41,y+9)),((x+6,y+13),(x+10,y+9)),((x+10,y+9),(x+15,y+12)),((x+15,y+12),(x+19,y+9)),((x+19,y+9),(x+23,y+12)),((x+22,y+2),(x+22,y+8)),((x+28,y+3),(x+26,y+8)),((x+32,y+4),(x+32,y+13)),((x+36,y+2),(x+40,y+6)),((x+4,y+9),(x+1,y+13))]
        for a,b in segs: draw.line((a[0],a[1]+1,b[0],b[1]+1), fill=shadow)
        for a,b in segs: draw.line((a[0],a[1],b[0],b[1]), fill=light)
    def draw_unbreakable(draw,x,y):
        outer=(186,196,206,255); inner=(58,68,82,255); square_shell(draw,x,y,44,16,outer,inner); draw.line((x+1,y+1,x+42,y+1), fill=(218,226,235,255)); draw.line((x+1,y+14,x+42,y+14), fill=(34,42,53,255)); draw.rectangle((x+3,y+3,x+40,y+12), fill=(76,88,103,255)); draw.rectangle((x+5,y+4,x+38,y+11), outline=(145,158,172,255), width=1); draw.rectangle((x+15,y+4,x+28,y+11), fill=(96,108,123,255), outline=(172,184,196,255), width=1)
        for bx,by in [(5,5),(37,5),(5,10),(37,10)]: draw.rectangle((x+bx,y+by,x+bx+1,y+by+1), fill=(220,228,236,255)); draw.point((x+bx+1,y+by+1), fill=(106,118,131,255))
        draw.line((x+7,y+6,x+13,y+6), fill=(42,50,62,255)); draw.line((x+7,y+9,x+13,y+9), fill=(116,128,142,255)); draw.line((x+30,y+6,x+36,y+6), fill=(42,50,62,255)); draw.line((x+30,y+9,x+36,y+9), fill=(116,128,142,255))
    # Restore the earlier 8.1-style hard-hit and metal-hit FX by re-drawing them
    starts=[0,58,116,174]; centers=[12,22,34,44]
    for sx,cx in zip(starts,centers):
        d.rectangle((sx,50,sx+55,77), fill=(0,0,0,0)); bx=sx+6; by=56
        rounded_shell(d,bx,by,44,16,(255,165,60,90),(186,84,22,70))
        for offset,color in [(-4,(255,230,120,80)),(-2,(255,235,150,140)),(0,(255,250,220,255)),(2,(255,235,150,140)),(4,(255,220,100,70))]: x=cx+offset+sx; d.rectangle((x,54,x+1,72), fill=color)
        d.line((sx+8,64,sx+48,64), fill=(255,214,110,110))
        for px,py in [(cx+sx-6,59),(cx+sx+6,60),(cx+sx-2,68),(cx+sx+9,67)]: d.point((px,py), fill=(255,245,200,220))
    starts=[0,58,116,174]; shine_x=[6,16,26,36]
    for sx,cx in zip(starts,shine_x):
        d.rectangle((sx,110,sx+55,137), fill=(0,0,0,0)); base_x=sx+6; base_y=116
        square_shell(d,base_x,base_y,44,16,(190,205,220,70),(78,92,108,60)); d.rectangle((base_x+3,base_y+3,base_x+40,base_y+12), fill=(94,108,124,60))
        for off,col in [(-5,(120,210,255,60)),(-3,(170,230,255,100)),(-1,(240,250,255,210)),(1,(170,230,255,100)),(3,(120,210,255,60))]: x=base_x+cx+off; d.rectangle((x,114,x+1,134), fill=col)
        d.line((base_x+4,124,base_x+39,124), fill=(190,235,255,80)); d.rectangle((base_x+5,118,base_x+38,131), outline=(220,245,255,70), width=1)
    d.rectangle((192,0,235,15), fill=(0,0,0,0)); d.rectangle((238,0,281,15), fill=(0,0,0,0)); draw_hard_damaged(d,192,0); draw_unbreakable(d,238,0)
    atlas.save(path)

patch_sprite_atlas_final_polish_2()


# --- Final polish pass 3: hard-hit FX, damaged hard cracks, green wide paddle
# This pass intentionally overrides a few atlas regions after all previous passes.
def patch_sprite_atlas_final_polish_3():
    from PIL import Image, ImageDraw
    path=os.path.join(ASSET,'sprites.png')
    atlas=Image.open(path).convert('RGBA')
    d=ImageDraw.Draw(atlas)

    def rounded_shell(draw,x,y,w,h, outer, inner):
        draw.rectangle((x+2,y,x+w-3,y), fill=outer)
        draw.rectangle((x+1,y+1,x+w-2,y+1), fill=outer)
        draw.rectangle((x,y+2,x+w-1,y+h-3), fill=outer)
        draw.rectangle((x+1,y+h-2,x+w-2,y+h-2), fill=outer)
        draw.rectangle((x+2,y+h-1,x+w-3,y+h-1), fill=outer)
        draw.rectangle((x+2,y+2,x+w-3,y+h-3), fill=inner)
        draw.rectangle((x+3,y+3,x+w-4,y+h-4), fill=inner)

    def draw_hard_damaged_black_cracks(draw,x,y):
        outer=(234,151,57,255); inner=(168,73,18,255); c1=(228,144,58,255); c2=(196,100,28,255); hi=(255,223,150,255); lo=(122,47,12,255)
        rounded_shell(draw,x,y,44,16,outer,inner)
        panel=(x+5,y+4,x+38,y+11)
        for sx in range(panel[0]-12,panel[2]+12,10): draw.polygon([(sx,panel[3]),(sx+5,panel[3]),(sx+13,panel[1]),(sx+8,panel[1])], fill=c1)
        for sx in range(panel[0]-7,panel[2]+17,10): draw.polygon([(sx,panel[3]),(sx+5,panel[3]),(sx+13,panel[1]),(sx+8,panel[1])], fill=c2)
        draw.rectangle(panel, outline=(255,232,150,255), width=1)
        draw.line((x+5,y+3,x+38,y+3), fill=hi)
        draw.line((x+5,y+12,x+38,y+12), fill=lo)
        crack=(0,0,0,255)
        segs=[((x+2,y+4),(x+8,y+6)),((x+8,y+6),(x+14,y+4)),((x+14,y+4),(x+18,y+8)),((x+18,y+8),(x+24,y+6)),((x+24,y+6),(x+30,y+10)),((x+30,y+10),(x+37,y+7)),((x+37,y+7),(x+41,y+9)),((x+6,y+13),(x+10,y+9)),((x+10,y+9),(x+15,y+12)),((x+15,y+12),(x+19,y+9)),((x+19,y+9),(x+23,y+12)),((x+22,y+2),(x+22,y+8)),((x+28,y+3),(x+26,y+8)),((x+32,y+4),(x+32,y+13)),((x+36,y+2),(x+40,y+6)),((x+4,y+9),(x+1,y+13))]
        for a,b in segs: draw.line((a[0],a[1],b[0],b[1]), fill=crack)

    def draw_wide_paddle_green(draw,x,y):
        outer=(118,255,168,255); inner=(28,190,118,255); shadow=(16,122,76,255); highlight=(214,255,228,255)
        w,h=128,12
        rounded_shell(draw,x,y,w,h,outer,inner)
        draw.line((x+6,y+2,x+w-7,y+2), fill=highlight)
        draw.line((x+6,y+h-3,x+w-7,y+h-3), fill=shadow)
        draw.rectangle((x+w//2-10,y+3,x+w//2+9,y+8), fill=(164,255,198,255), outline=(34,144,92,255))
        for sx in (x+18, x+w-19):
            draw.line((sx,y+3,sx,y+8), fill=(34,144,92,255))
            draw.line((sx+1,y+3,sx+1,y+8), fill=(160,250,190,255))

    def draw_hard_hit_particles(draw, sx, sy, frame):
        draw.rectangle((sx,sy,sx+55,sy+27), fill=(0,0,0,0))
        cx=sx+28; cy=sy+14
        white=(255,255,255,255); soft=(255,255,255,170); dim=(255,255,255,110)
        if frame == 0:
            for p in [(cx,cy),(cx-1,cy),(cx+1,cy),(cx,cy-1),(cx,cy+1)]: draw.point(p, fill=white)
            for p in [(cx-4,cy-2),(cx+4,cy-1),(cx-3,cy+3),(cx+3,cy+2)]: draw.point(p, fill=soft)
        elif frame == 1:
            for p in [(cx-6,cy-4),(cx+6,cy-4),(cx-6,cy+4),(cx+6,cy+4)]: draw.rectangle((p[0]-1,p[1]-1,p[0],p[1]), fill=white)
            for p in [(cx-3,cy-7),(cx+4,cy-7),(cx-4,cy+7),(cx+3,cy+7),(cx,cy)]: draw.point(p, fill=soft)
        elif frame == 2:
            for p in [(cx-10,cy-6),(cx+10,cy-6),(cx-10,cy+6),(cx+10,cy+6)]: draw.rectangle((p[0]-1,p[1]-1,p[0],p[1]), fill=white)
            for p in [(cx-14,cy),(cx+14,cy),(cx,cy-9),(cx,cy+9),(cx-5,cy-3),(cx+5,cy+3)]: draw.point(p, fill=soft)
        else:
            for p in [(cx-15,cy-8),(cx+15,cy-8),(cx-15,cy+8),(cx+15,cy+8)]: draw.point(p, fill=soft)
            for p in [(cx-11,cy-10),(cx+11,cy-10),(cx-11,cy+10),(cx+11,cy+10),(cx-18,cy),(cx+18,cy)]: draw.point(p, fill=dim)

    d.rectangle((192,0,235,15), fill=(0,0,0,0))
    draw_hard_damaged_black_cracks(d,192,0)
    d.rectangle((284,0,411,11), fill=(0,0,0,0))
    draw_wide_paddle_green(d,284,0)
    starts=[0,58,116,174]
    for i,sx in enumerate(starts):
        draw_hard_hit_particles(d, sx, 50, i)
    atlas.save(path)

patch_sprite_atlas_final_polish_3()


# --- Final polish pass 4: clip hard-block stripes to their own atlas cells --
def patch_sprite_atlas_final_polish_4():
    from PIL import Image, ImageDraw
    path=os.path.join(ASSET,'sprites.png')
    atlas=Image.open(path).convert('RGBA')
    d=ImageDraw.Draw(atlas)
    d.rectangle((146,0,281,15), fill=(0,0,0,0))

    def rounded_shell(draw,x,y,w,h, outer, inner):
        draw.rectangle((x+2,y,x+w-3,y), fill=outer); draw.rectangle((x+1,y+1,x+w-2,y+1), fill=outer); draw.rectangle((x,y+2,x+w-1,y+h-3), fill=outer); draw.rectangle((x+1,y+h-2,x+w-2,y+h-2), fill=outer); draw.rectangle((x+2,y+h-1,x+w-3,y+h-1), fill=outer); draw.rectangle((x+2,y+2,x+w-3,y+h-3), fill=inner); draw.rectangle((x+3,y+3,x+w-4,y+h-4), fill=inner)
    def square_shell(draw,x,y,w,h, outer, inner):
        draw.rectangle((x,y,x+w-1,y+h-1), fill=outer); draw.rectangle((x+1,y+1,x+w-2,y+h-2), fill=inner); draw.rectangle((x+2,y+2,x+w-3,y+h-3), fill=inner)
    def stripe_panel(x0,y0,x1,y1,c1,c2):
        pw=x1-x0+1; ph=y1-y0+1
        panel=Image.new('RGBA',(pw,ph),(0,0,0,0)); pd=ImageDraw.Draw(panel)
        for sx in range(-14,pw+14,10): pd.polygon([(sx,ph-1),(sx+5,ph-1),(sx+13,0),(sx+8,0)], fill=c1)
        for sx in range(-9,pw+19,10): pd.polygon([(sx,ph-1),(sx+5,ph-1),(sx+13,0),(sx+8,0)], fill=c2)
        atlas.alpha_composite(panel,(x0,y0))
    def draw_hard(x,y,damaged=False):
        if damaged:
            outer=(234,151,57,255); inner=(168,73,18,255); c1=(228,144,58,255); c2=(196,100,28,255); hi=(255,223,150,255); lo=(122,47,12,255)
        else:
            outer=(255,188,78,255); inner=(212,99,22,255); c1=(255,178,78,255); c2=(233,127,33,255); hi=(255,238,180,255); lo=(158,62,15,255)
        rounded_shell(d,x,y,44,16,outer,inner); stripe_panel(x+5,y+4,x+38,y+11,c1,c2); d.rectangle((x+5,y+4,x+38,y+11), outline=(255,232,150,255), width=1); d.line((x+5,y+3,x+38,y+3), fill=hi); d.line((x+5,y+12,x+38,y+12), fill=lo)
        if damaged:
            crack=(0,0,0,255); segs=[((x+2,y+4),(x+8,y+6)),((x+8,y+6),(x+14,y+4)),((x+14,y+4),(x+18,y+8)),((x+18,y+8),(x+24,y+6)),((x+24,y+6),(x+30,y+10)),((x+30,y+10),(x+37,y+7)),((x+37,y+7),(x+41,y+9)),((x+6,y+13),(x+10,y+9)),((x+10,y+9),(x+15,y+12)),((x+15,y+12),(x+19,y+9)),((x+19,y+9),(x+23,y+12)),((x+22,y+2),(x+22,y+8)),((x+28,y+3),(x+26,y+8)),((x+32,y+4),(x+32,y+13)),((x+36,y+2),(x+40,y+6)),((x+4,y+9),(x+1,y+13))]
            for a,b in segs: d.line((*a,*b), fill=crack)
    def draw_unbreakable(x,y):
        outer=(186,196,206,255); inner=(58,68,82,255); square_shell(d,x,y,44,16,outer,inner); d.line((x+1,y+1,x+42,y+1), fill=(218,226,235,255)); d.line((x+1,y+14,x+42,y+14), fill=(34,42,53,255)); d.rectangle((x+3,y+3,x+40,y+12), fill=(76,88,103,255)); d.rectangle((x+5,y+4,x+38,y+11), outline=(145,158,172,255), width=1); d.rectangle((x+15,y+4,x+28,y+11), fill=(96,108,123,255), outline=(172,184,196,255), width=1)
        for bx,by in [(5,5),(37,5),(5,10),(37,10)]: d.rectangle((x+bx,y+by,x+bx+1,y+by+1), fill=(220,228,236,255)); d.point((x+bx+1,y+by+1), fill=(106,118,131,255))
        d.line((x+7,y+6,x+13,y+6), fill=(42,50,62,255)); d.line((x+7,y+9,x+13,y+9), fill=(116,128,142,255)); d.line((x+30,y+6,x+36,y+6), fill=(42,50,62,255)); d.line((x+30,y+9,x+36,y+9), fill=(116,128,142,255))
    draw_hard(146,0,False); draw_hard(192,0,True); draw_unbreakable(238,0)
    atlas.save(path)

patch_sprite_atlas_final_polish_4()


# --- Milestone 9 overlay sprite: laser cannon -------------------------------
def patch_laser_cannon_sprite():
    from PIL import Image, ImageDraw
    path=os.path.join(ASSET,'sprites.png')
    atlas=Image.open(path).convert('RGBA')
    d=ImageDraw.Draw(atlas)
    d.rectangle((232,20,239,29), fill=(0,0,0,0))
    d.rectangle((233,26,238,29), fill=(34,79,92,255))
    d.rectangle((232,27,239,29), fill=(20,52,66,255))
    d.rectangle((234,20,237,27), fill=(82,205,225,255))
    d.rectangle((235,20,236,24), fill=(224,252,255,255))
    d.rectangle((233,23,233,26), fill=(50,126,144,255))
    d.rectangle((238,23,238,26), fill=(50,126,144,255))
    d.rectangle((233,20,238,21), fill=(156,240,245,255))
    d.rectangle((234,19,237,20), fill=(225,255,255,255))
    atlas.save(path)

patch_laser_cannon_sprite()


# --- Final polish pass 4: longer laser cannon overlay ----------------------
def patch_sprite_atlas_final_polish_4():
    from PIL import Image, ImageDraw
    path=os.path.join(ASSET, 'sprites.png')
    atlas=Image.open(path).convert('RGBA')
    d=ImageDraw.Draw(atlas)
    d.rectangle((232,20,239,33), fill=(0,0,0,0))
    x,y=232,20
    outline=(40,92,138,255)
    body=(86,214,255,255)
    high=(220,248,255,255)
    dark=(26,130,176,255)
    accent=(255,255,255,255)
    for yy in range(y, y+4):
        d.line((x+2,yy,x+5,yy), fill=outline if yy==y else body)
    d.point((x+3,y), fill=accent)
    d.point((x+4,y), fill=accent)
    for yy in range(y+4, y+6):
        d.line((x+1,yy,x+6,yy), fill=body)
    for yy in range(y+6, y+14):
        d.line((x,yy,x+7,yy), fill=body)
    for yy in range(y+1, y+14):
        d.point((x+2,yy), fill=high)
        d.point((x+3,yy), fill=high if yy<y+9 else body)
        d.point((x+6,yy), fill=dark)
    for yy in range(y+7, y+12):
        d.point((x+4,yy), fill=outline)
    for px,py in [(x+1,y+7),(x+1,y+9),(x+1,y+11),(x+5,y+8),(x+5,y+10)]:
        d.point((px,py), fill=outline)
    for xx in range(x+1,x+7):
        d.point((xx,y+13), fill=dark if xx in (x+1, x+6) else outline)
    for p in [(x+1,y+3),(x+6,y+3),(x+2,y+5),(x+5,y+5)]:
        d.point(p, fill=outline)
    atlas.save(path)

patch_sprite_atlas_final_polish_4()


# --- Final polish pass 5: gray/black thicker laser cannons ----------------
def patch_sprite_atlas_final_polish_5():
    from PIL import Image, ImageDraw
    path=os.path.join(ASSET, 'sprites.png')
    atlas=Image.open(path).convert('RGBA')
    d=ImageDraw.Draw(atlas)
    d.rectangle((232,18,241,33), fill=(0,0,0,0))
    x,y=232,18
    black=(0,0,0,255); dk=(78,84,92,255); mid=(146,154,164,255); lt=(205,212,220,255)
    for yy in range(y, y+4): d.line((x+2,yy,x+7,yy), fill=black if yy==y else mid)
    for p in [(x+4,y),(x+5,y)]: d.point(p, fill=lt)
    for yy in range(y+4,y+6): d.line((x+1,yy,x+8,yy), fill=mid)
    for yy in range(y+6,y+16): d.line((x,yy,x+9,yy), fill=mid)
    for yy in range(y+1,y+16): d.point((x,yy), fill=black); d.point((x+9,yy), fill=black)
    for xx in range(x+1,x+9): d.point((xx,y+15), fill=black)
    for yy in range(y+1,y+15):
        d.point((x+2,yy), fill=lt); d.point((x+3,yy), fill=lt if yy<y+10 else mid); d.point((x+7,yy), fill=dk); d.point((x+8,yy), fill=black)
    for yy in range(y+7,y+13): d.point((x+5,yy), fill=black)
    for p in [(x+1,y+7),(x+1,y+10),(x+2,y+12),(x+6,y+8),(x+6,y+11),(x+4,y+5),(x+5,y+5),(x+3,y+3),(x+6,y+3)]: d.point(p, fill=black)
    for xx in range(x+1,x+9): d.point((xx,y+14), fill=dk if xx not in (x+1,x+8) else black)
    atlas.save(path)

patch_sprite_atlas_final_polish_5()
