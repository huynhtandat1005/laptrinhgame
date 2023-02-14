bg = main_img
        
        if event.type == pygame.MOUSEBUTTONDOWN:
            if 575 <= mouse[0] <= 875 and 360 <= mouse[1] <= 660:
                state = "play"

        if event.type == pygame.MOUSEBUTTONDOWN:
            if 1400 <= mouse[0] <= 1500 and 0 <= mouse[1] <= 100:
                if sound_img == bs_on:
                    pygame.mixer.music.stop()
                    sound_img = bs_off
                elif sound_img == bs_off:
                    pygame.mixer.music.play()
                    sound_img = bs_on