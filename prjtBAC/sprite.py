import pygame
import globalVar

class Animation:
    """ Classe qui comporte une ou plusieurs textures, qui s'enchainent en fonction de la période de rafraichissement"""
    def __init__(self, arrSurf = [], refreshPeriod = 1, r = True, destroyAtEnd = False):
        self.surf =  arrSurf.copy()  # Liste des textures 
        self.refreshPeriod = refreshPeriod # Période 
        self.repeat = r  # Si l'animation se répète, ou s'arrête à la fin
        self.frameStart = 0 # Numéro de l'image de la dernière rafraichissement
        self.surfUsed = 0 # Numero de la texture utilisée
        self.destroyAtEnd = destroyAtEnd # Si l'objet est supprimé quand l'animation se termine

    def isOver(self):
        """Si l'animation est arrivée à la dernière texture """
        return self.surfUsed == len(self.surf)-1

    def start(self):
        """ Initialise l'animation"""
        self.frameStart = globalVar.FRAME
        self.surfused = 0

    def cycle(self):
        """ Passe à la texture suivante si la période d'un texture est finie"""
        if( not(self.repeat) and self.isOver() ): return # Interruption de la fonction 
        if(globalVar.FRAME - self.frameStart > self.refreshPeriod):
            self.surfUsed = (self.surfUsed+1) %len(self.surf)
            self.frameStart = globalVar.FRAME

    def getUsedSurf(self):
        """Retourne la texture actuellement utilisée par l'animation """
        return self.surf[self.surfUsed]


class Sprite:
    """ Classe qui comporte une ou plusieurs objets Animation, qui sont utilisés en fonction de l'état définie"""
    def __init__(self, arrAnim = []):
        self.anim = arrAnim.copy() # liste des animations
        self.state = 0

    def changeState(self, state):
        """ Change l'état, et initialise l'animation correspondant """
        if state != self.state:
            self.state = state
            self.anim[abs(self.state)].start()

    def cycle(self):
        self.anim[abs(self.state)].cycle()

    def getUsedSurf(self):
        """ Retourne la texture de l'animation. Si state est négatif, on utilise une reflexion horizontale de la texture """
        if self.state < 0: 
            return pygame.transform.flip( self.anim[abs(self.state)].getUsedSurf() , True, False) # xbool = True, ybool = False
        return self.anim[self.state].getUsedSurf()

def makeRectSpr(size, color):
    """Crée un objet Sprite qui comporte un objet Animation, qui comporte un rectangle coloré """
    surf = pygame.Surface(size)
    surf.fill(color)
    return  Sprite([Animation([surf])])

# Création des sprites du jeu
tmp = pygame.image.load("sprites/Player/jump.png") 
tmp.set_colorkey((0,0,0)) # Le fond noir de l'image est converti au transparent
aniJump = Animation([tmp]) 
tmp = pygame.image.load("sprites/Player/idle.png")  
tmp.set_colorkey((0,0,0))
aniIdle  = Animation([tmp])
parentSurfWalk = pygame.image.load("sprites/Player/jog.png") # Image qui contient plusieurs textures de l'animation de marche
parentSurfWalk.set_colorkey((0,0,0)) 
aniWalk = Animation(refreshPeriod = 5)
for i in range(6):
    aniWalk.surf.append( parentSurfWalk.subsurface( pygame.Rect(64*i,0, 64,64) )) # Création des sous-textures, on prenant plusieurs parties de l'image de base 
sprPlayer = Sprite([aniIdle, aniWalk, aniJump]) # Sprite du joueur

parentSurfBullet = pygame.image.load("sprites/Bullet/bullet.png") 
aniBullet = Animation(refreshPeriod = 5)
for i in range(2):
    aniBullet.surf.append( parentSurfBullet.subsurface( pygame.Rect(5*i,0, 5,5) ) ) 
sprBullet = Sprite([aniBullet]) # Sprite d'une balle

del parentSurfWalk, tmp, aniIdle, aniWalk, aniJump, parentSurfBullet, aniBullet # Suppression de tout ce qui est non-nécessaire

def test(): # Fonction pour tester rapidement les sprites crées
    pygame.init()
    d = True
    S = pygame.display.set_mode( (100 ,100) )
    while d:
        pygame.time.Clock().tick(60)
        for e in pygame.event.get(): 
            if e.type == pygame.QUIT: d = False
        S.fill((0,0,0))
        sprPlayer.cycle()
        sprPlayer.changeState(-1)
        S.blit(sprPlayer.getUsedSurf(), (0,0))
        pygame.display.flip()
        globalVar.FRAME += 1
    pygame.quit()