import pygame

# init pygame
pygame.init()

# Set up display
width, height = 1200, 1200
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Data Preprocessing")

# Colors
BLACK = (156, 84, 84)
WHITE = (255, 216, 216)

# Set chess board dimensions
board_size = 8
tile_size = height // (board_size * 3.5)

# Calculate the new size based on 90% of tile size
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
  print(f"Board config: {board_configs}")
  for level, board_config in enumerate(board_configs):
    for row in range(len(board_config)):
      for col in range(len(board_config[row])):
        piece = int(board_config[row][col].strip())
        print(f"piece: {piece} | row: {row} | col: {col}")
        if piece != 0 and piece in pieces:
          x = col * tile_size
          x += 400
          y = level * (board_size * tile_size) + row * tile_size
          y += 50
          sprite = pygame.sprite.Sprite()
          sprite.image = pieces[piece]
          sprite.rect = sprite.image.get_rect()
          sprite.rect.x = x
          sprite.rect.y = y
          sprites.add(sprite)
  return sprites

# Main game loop
def game_loop():
  # Load board configs
  board_configs = load_board_configs("/home/zbostock/Projects/3DChess/src/move_data/chess_poss.csv")
  if not board_configs:
    return

  # Set up sprites
  current_board = 0
  sprites = create_sprites(board_configs[current_board])

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
          # Clear the sprites and load the new board config
          sprites.empty()
          sprites = create_sprites([board_configs[current_board]])


    # Clear the screen
    screen.fill((0,0,0))

    # Draw chess board
    for level in range(3):
      for row in range(board_size):
        for col in range(board_size):
          x = col * tile_size
          x += 400
          y = level * (board_size * tile_size) + row * tile_size
          if level == 1:
            y += 50
          elif level == 2:
            y += 100
          color = WHITE if (row + col) % 2 == 0 else BLACK
          pygame.draw.rect(screen, color, (x, y, tile_size, tile_size))

    # Draw pieces
    sprites.update()
    sprites.draw(screen)

    pygame.display.flip()
    clock.tick(60)

  pygame.quit()

game_loop()
