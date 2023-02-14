# PUNCH ZOMBIE GAME BY PYGAME
import pygame
pygame.init()

screen = pygame.display.set_mode([1500, 800])
icon = pygame.image.load('icon.png')
main_img = pygame.image.load('main.png')
play_img = pygame.image.load('background.png')
bp = pygame.image.load('button_play.png')
bs_on = pygame.image.load('button_sound-on.png')
bs_off = pygame.image.load('button_sound-off.png')
bn = pygame.image.load('button_notice.png')
tt = pygame.image.load('title.png')
music_bg = 'nhacnen.mp3'
music_play = 'nhacchoi.mp3'

pygame.display.set_icon(icon)
pygame.display.set_caption('PUNCH ZOMBIE')
sound_img = bs_on
state = 'main'
pygame.mixer.music.load(music_bg)
pygame.mixer.music.play()
music_on = 1

running = True
while running:
    for event in pygame.event.get():
        # Close if the user quits the game
        if event.type == pygame.QUIT:
            running = False
        
        if state == 'main':
            screen.blit(main_img,(0,0))
            screen.blit(bp,(575,360))
            screen.blit(bn,(10,10))
            screen.blit(tt,(300,90))
            screen.blit(sound_img,(1400,0))
            
            if event.type == pygame.MOUSEBUTTONDOWN:
                if 575 <= mouse[0] <= 875 and 360 <= mouse[1] <= 660:
                    state = 'play'

            if event.type == pygame.MOUSEBUTTONDOWN:
                if 1400 <= mouse[0] <= 1500 and 0 <= mouse[1] <= 100:
                    if sound_img == bs_on:
                        pygame.mixer.music.pause()
                        sound_img = bs_off
                    elif sound_img == bs_off:
                        pygame.mixer.music.unpause()
                        sound_img = bs_on  
                        
        elif state == 'play':
            if music_on == 1:
                pygame.mixer.music.load(music_play)
                pygame.mixer.music.play()
                music_on = 0

            screen.blit(play_img,(0,0))
            screen.blit(sound_img,(1400,0))
            if event.type == pygame.MOUSEBUTTONDOWN:
                if 1400 <= mouse[0] <= 1500 and 0 <= mouse[1] <= 100:
                    if sound_img == bs_on:
                        pygame.mixer.music.pause()
                        sound_img = bs_off
                    elif sound_img == bs_off:
                        pygame.mixer.music.unpause()
                        sound_img = bs_on
                        
    mouse = pygame.mouse.get_pos()
    pygame.display.update()

# Quit the GUI game
pygame.quit()