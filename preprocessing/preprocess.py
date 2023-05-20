import pygame
import numpy as np
from arrow import draw_arrow
import os.path

# init pygame
pygame.init()

# Set up display
width, height = 1200, 1000
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Data Preprocessing")

# Colors
BLACK = (156, 84, 84)
WHITE = (255, 216, 216)
RED = (255, 0, 0)
BLUE = (0, 0, 255)

# Set chess board dimensions
board_size = 8
tile_size = height // (board_size * 3.5)

# Calculate the new size based on 100% of tile size
new_size = int(tile_size * 1)

# Load chess piece images
pieces = {
    1: pygame.transform.scale(pygame.image.load("sprites/w_bishop.png"), (new_size, new_size)),
    2: pygame.transform.scale(pygame.image.load("sprites/w_rook.png"), (new_size, new_size)),
    3: pygame.transform.scale(pygame.image.load("sprites/w_queen.png"), (new_size, new_size)),
    4: pygame.transform.scale(pygame.image.load("sprites/w_pawn.png"), (new_size, new_size)),
    5: pygame.transform.scale(pygame.image.load("sprites/w_knight.png"), (new_size, new_size)),
    6: pygame.transform.scale(pygame.image.load("sprites/w_king.png"), (new_size, new_size)),
    7: pygame.transform.scale(pygame.image.load("sprites/b_bishop.png"), (new_size, new_size)),
    8: pygame.transform.scale(pygame.image.load("sprites/b_rook.png"), (new_size, new_size)),
    9: pygame.transform.scale(pygame.image.load("sprites/b_queen.png"), (new_size, new_size)),
    10: pygame.transform.scale(pygame.image.load("sprites/b_pawn.png"), (new_size, new_size)),
    11: pygame.transform.scale(pygame.image.load("sprites/b_knight.png"), (new_size, new_size)),
    12: pygame.transform.scale(pygame.image.load("sprites/b_king.png"), (new_size, new_size)),
    0: None,  # Empty square
}

# Load chess board configurations from file
def load_board_configs(file_name):
    try:
        with open(file_name, "r") as file:
            lines = file.readlines()
            return [line.strip().split(",") for line in lines]
    except IOError:
        print(f"Error: Could not open file '{file_name}'")
        return None

# Create sprites for chess pieces
def create_sprites(board_configs):
    sprites = pygame.sprite.Group()
    board = np.asarray(board_configs).squeeze()
    for idx in range(len(board)):
        piece = int(board[idx])
        level = idx // 64
        if piece != 0 and piece in pieces:
            x = (idx % 8) * tile_size
            x += 400
            y = (board_size) + (idx // 8) * tile_size
            y += 40
            if level == 0:
                y -= 50
            elif level == 2:
                y += 50
            sprite = pygame.sprite.Sprite()
            sprite.image = pieces[piece]
            sprite.rect = sprite.image.get_rect()
            sprite.rect.x = x
            sprite.rect.y = y
            sprites.add(sprite)
    return sprites

# Create arrow
def create_arrow(from_square, to_square):
    level_from, level_to = (from_square // 64), (to_square // 64)
    from_x = (from_square % 8) * tile_size + 400 + (tile_size * 1.5)
    from_y = (from_square // 8) * tile_size + tile_size + 25
    to_x = (to_square % 8) * tile_size + 400 + (tile_size * 0.5)
    to_y = (to_square // 8) * tile_size + tile_size + 25
    if (level_from > level_to):
      to_y -= 50
    elif (level_to > level_from):
      to_y += 50
    #print(f"from_x: {from_x} | from_y: {from_y} | to_x: {to_x} | to_y: {to_y}")
    start = pygame.Vector2(from_x, from_y)
    end = pygame.Vector2(to_x, to_y)
    #pygame.draw.circle(screen, RED, start, (tile_size / 2), width = 2)
    #pygame.draw.circle(screen, RED, end, (tile_size / 2), width = 2)
    #draw_arrow(screen, start, end, pygame.Color("dodgerblue"), 10, 20, 12)

# Main game loop
def game_loop():
    bc_filepath = os.path.expanduser("../src/move_data/chess_poss.csv")
    fp_filepath = os.path.expanduser("../src/move_data/from_move_only.csv")
    tp_filepath = os.path.expanduser("../src/move_data/lables.csv")
    all_filepath = os.path.expanduser("../src/move_data/all.csv")

    # Load board configs
    board_configs = load_board_configs(bc_filepath)
    if not board_configs:
        return

    # Load from_poss configs
    from_poss = load_board_configs(fp_filepath)
    if not from_poss:
        return

    # Load to_poss configs
    to_poss = load_board_configs(tp_filepath)
    if not to_poss:
        return

    all_poss = load_board_configs(all_filepath)
    if not all_poss:
      return

    # Set up sprites
    current_board = 0
    num_saved = 0
    sprites = create_sprites(board_configs[current_board])
    previous_boards = []  # List to store previous board configurations
    previous_boards.append(board_configs[current_board])

    clock = pygame.time.Clock()
    running = True

    # game loop
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    # Move to the next line in the file
                    current_board = (current_board + 1) % len(board_configs)
                elif event.key == pygame.K_BACKSPACE:
                    if len(previous_boards) > 1:
                        previous_boards.pop()
                        current_board = (current_board - 1) % len(board_configs)
                elif event.key == pygame.K_p:
                    num_saved += 1
                    # Save the current board configuration to "passed.csv" file
                    with open("passed.csv", "a") as file:
                        file.write(",".join(all_poss[current_board]))
                        file.write("\n")
                    # Move to the next line in the file
                    current_board = (current_board + 1) % len(board_configs)
                elif event.key == pygame.K_q:
                    # Move to the next line in the file
                    current_board = (current_board + 1) % len(board_configs)
                elif event.key == pygame.K_ESCAPE:
                    running = False

                # Clear the sprites and load the new board config
                sprites.empty()
                sprites = create_sprites([board_configs[current_board]])
                previous_boards.append(board_configs[current_board])
            """
            elif event.type == pygame.MOUSEMOTION:
              # Display mouse coordinates in the terminal
              x, y = event.pos
              print(f"Mouse coordinates: x={x}, y={y}")
            """
        # Clear the screen
        screen.fill((0, 0, 0))
        from_square = int(from_poss[current_board][0])
        to_square = int(to_poss[current_board][0])

        # Draw chess board
        cur_square = 0
        for level in range(3):
            for row in range(board_size):
                for col in range(board_size):
                    cur_square += 1
                    x = col * tile_size
                    x += 400
                    y = level * (board_size * tile_size) + row * tile_size
                    if level == 1:
                        y += 50
                    elif level == 2:
                        y += 100
                    color = WHITE if (row + col) % 2 == 0 else BLACK
                    if level == 0:
                      if cur_square - 1 == from_square:
                        color = BLUE
                      elif cur_square - 1 == to_square:
                        color = RED
                    elif level == 1:
                      if cur_square - 2 == from_square:
                        color = BLUE
                      elif cur_square - 2 == to_square:
                        color = RED
                    elif level == 2:
                      if cur_square - 3 == from_square:
                        color = BLUE
                      elif cur_square - 3 == to_square:
                        color = RED
                    pygame.draw.rect(screen, color, (x, y, tile_size, tile_size))

        # Draw the chess pieces and arrow
        sprites.draw(screen)
        sprites.update()
        if current_board < len(from_poss) and current_board < len(to_poss):
          create_arrow(from_square, to_square)

        # Render the current board number
        font = pygame.font.Font(None, 36)
        text = font.render(f"Board: {current_board + 1} / {len(board_configs)}", True, WHITE)
        saved = font.render(f"Number Saved: {num_saved}", True, WHITE)
        screen.blit(text, (20, 20))
        screen.blit(saved, (20, 60))

        # Update the screen
        pygame.display.flip()
        clock.tick(60)
    pygame.quit()

# Run the game loop
game_loop()
