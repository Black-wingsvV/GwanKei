#include <cassert>
#include "game.hpp"


namespace GwanKei {

  bool is_valid_game_piece_id(int id) {
    return (0 <= id && id <= 1+(25+1)*4);
  }

  bool is_valid_layout_index(int index) {
    return (0 <= index && index < 25);
  }

  bool is_valid_layout(Piece *data) {
    int count[42] = {0};
    for(int i=0; i<25; i++) {
      count[data[i].get_id()]++;
    }
    for(int i=0; i<12; i++) {
      if(count[PIECES[i]] != NUMBER_OF_PIECES[PIECES[i]])
	return false;
    }
    return true;
  }


  Orient convert_player_to_orient(Player player) {
    return static_cast<Orient>(player+1);
  }

  Player convert_orient_to_player(Orient orient) {
    assert(orient != Central);
    return static_cast<Player>(orient-1);
  }

  Cell convert_layout_index_to_cell(int index, CellGroup group /* = South */) {
    assert(is_valid_layout_index(index));
    return Cell(
	        group, Y_OF_INDEX[index], X_OF_INDEX[index], LR_OF_INDEX[index]
	   );
  }

  int convert_cell_to_layout_index(const Cell& cell) {
    assert(cell.get_group() != Central);
    int t = 18*cell.get_lr()+6*(cell.get_x()-1) + cell.get_y()-1;
    // Camp = -1
    return INDEX_OF_CELL[t];
  }

  Layout::Layout(bool masked /* = false */) {
    assert(is_valid_layout(this->data));
    this->masked = masked;
  }

  Layout::Layout(Piece *data) {
    assert(is_valid_layout(data));
    for(int i=0; i<25; i++)
      this->data[i] = data[i];
  }

  bool Layout::is_masked() const {
    return this->masked;
  }

  Piece Layout::get(int index) const {
    assert(!is_masked());
    return data[index];
  }

  Piece Layout::get(int y, int x, LeftRight lr) const {
    assert(!is_masked());
    return data[convert_cell_to_layout_index(Cell(South,y,x,lr))];
  }

  void Layout::swap(int index1, int index2) {
    assert(!is_masked());
    Piece t = data[index1];
    data[index1] = data[index2];
    data[index2] = t;
  }

  std::string Layout::to_string() const {
    std::string result = "";
    if(is_masked()) {
      result = "[Masked]";
    } else {
      for(int y=1; y<=6; y++) {
	for(int t=1; t<=5; t++) {
	  int x;
	  LeftRight lr;
	  if(t <= 3) {
	    x = t;
	    lr = Left;
	  } else if(t == 4) {
	    x = 2;
	    lr = Right;
	  } else {
	    x = 1;
	    lr = Right;
	  }
	  int index = convert_cell_to_layout_index(Cell(South,y,x,lr));
	  if(index == -1)
	    result += "[  ]";
	  else if(data[index].get_id() == 0)
	    result += "[00]";
	  else
	    result += "[" + std::to_string(data[index].get_id()) + "]";
	  result += " ";
	} // for t in 1..5
	result += "\n";
      } // for y in 1..6
    } // masked ?
    return result;
  }

  Layout& Layout::operator = (const Layout& right) {
    for(int i=0; i<25; i++)
      data[i] = right.data[i];
    this->masked = right.masked;
    return *this;
  }
}