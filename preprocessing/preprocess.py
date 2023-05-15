import pygame

# init pygame
pygame.init()

# Set up display
width, height = 640, 640
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Data Preprocessing")

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

# Set chess board dimensions
board_size = 8
tile_size = height // (board_size * 3)

# Load chess piece images
pieces = {
  1: pygame.image.load("/home/zbostock/3DChess/preprocessing/sprites/w_bishop.png"),
  2: pygame.image.load("/home/zbostock/3DChess/preprocessing/sprites/w_rook.png"),
  3: pygame.image.load("/home/zbostock/3DChess/preprocessing/sprites/w_queen.png"),
  4: pygame.image.load("/home/zbostock/3DChess/preprocessing/sprites/w_pawn.png"),
  5: pygame.image.load("/home/zbostock/3DChess/preprocessing/sprites/w_knight.png"),
  6: pygame.image.load("/home/zbostock/3DChess/preprocessing/sprites/w_king.png"),
  7: pygame.image.load("/home/zbostock/3DChess/preprocessing/sprites/b_bishop.png"),
  8: pygame.image.load("/home/zbostock/3DChess/preprocessing/sprites/b_rook.png"),
  9: pygame.image.load("/home/zbostock/3DChess/preprocessing/sprites/b_queen.png"),
  10: pygame.image.load("/home/zbostock/3DChess/preprocessing/sprites/b_pawn.png"),
  11: pygame.image.load("/home/zbostock/3DChess/preprocessing/sprites/b_knight.png"),
  12: pygame.image.load("/home/zbostock/3DChess/preprocessing/sprites/b_king.png"),
  0: None, # Empty square
}

# Load chess board configurations from file
def load_board_configs(file_name):
  try:
    with open(file_name, "r") as file:
      line = file.readline()
      pieces_list = line.strip().split(",")
      board_configs = [pieces_list[i:i + board_size] for i in range(0, len(pieces_list), board_size)]
      # Print board_configs
      print("Board Configurations:")
      for config in board_configs:
        print(config)
      return [board_configs]
  except IOError:
    print(f"Error: Could not open file '{file_name}'")
    return None

# Create sprites for chess pieces
def create_sprites(board_configs):
  sprites = pygame.sprite.Group()
  for level, board_config in enumerate(board_configs):
    for row in range(board_size):
      for col in range(board_size):
        piece = int(board_config[row][col].strip())
        print(f"Piece number: {piece}")
        if piece != 0 and piece in pieces:
          x = col * tile_size
          #y = (level * board_size * tile_size * 3) + (row * tile_size)
          y = (level * board_size + row) * tile_size * 3
          sprite = pygame.sprite.Sprite()
          sprite.image = pieces[piece]
          sprite.rect = sprite.image.get_rect()
          sprite.rect.x = x
          sprite.rect.y = y
          sprites.add(sprite)
  return sprites

# Main game loop
def game_loop():
  screen_width, screen_height = 1920, 1080
  # Load board configs
  board_configs = load_board_configs("/home/zbostock/move_data/chess_poss.csv")
  if not board_configs:
    return

  # init pygame
  screen = pygame.display.set_mode((screen_width, screen_height))
  pygame.display.set_caption("Data Preprocessing")

  # Set up sprites
  sprites = create_sprites(board_configs)

  clock = pygame.time.Clock()
  running = True

  # game loop
  while running:
    for event in pygame.event.get():
      if event.type == pygame.QUIT:
        running = False

    # Clear the screen
    screen.fill(BLACK)

    # Draw chess board
    for level, board_config in enumerate(board_configs):
      for row in range(board_size):
        for col in range(board_size):
          x = col * tile_size
          y = level * (board_size * tile_size) + row * tile_size
          color = WHITE if (row + col) % 2 == 0 else BLACK
          pygame.draw.rect(screen, color, (x, y, tile_size, tile_size))

    # Draw pieces
    sprites.update()
    sprites.draw(screen)

    pygame.display.flip()
    clock.tick(60)

  pygame.quit()

game_loop()
