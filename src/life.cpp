#include <gecode/driver.hh>
#include <gecode/minimodel.hh>

using namespace Gecode;

class Life: public Script {
private:
	const int n;
	BoolVarArray gridVector;
public:
	Life(const SizeOptions& opt) :
			Script(opt), n(opt.size()), gridVector(*this, (n + 4) * (n + 4), 0, 1) {

		Matrix<BoolVarArray> grid(gridVector, n + 4, n + 4);

		// Borders should be zero
		rel(*this, grid.row(0), IRT_EQ, 0);
		rel(*this, grid.row(1), IRT_EQ, 0);
		rel(*this, grid.row(n + 2), IRT_EQ, 0);
		rel(*this, grid.row(n + 3), IRT_EQ, 0);

		rel(*this, grid.col(0), IRT_EQ, 0);
		rel(*this, grid.col(1), IRT_EQ, 0);
		rel(*this, grid.col(n + 2), IRT_EQ, 0);
		rel(*this, grid.col(n + 3), IRT_EQ, 0);

		for (int i = 2; i < n; i++) {
			for (int j = 2; j < n; j++) {
				BoolVar current = grid(i, j);
				BoolVar up = grid(i, j - 1);
				BoolVar down = grid(i, j + 1);
				BoolVar left = grid(i - 1, j);
				BoolVar right = grid(i + 1, j);

				std::cout << "UP: " << up << "DOWN: " << down << "LEFT: " << left << "RIGHT: " << right << std::endl;

				// TODO None is alive. It is NOT working
				if (current.one()) {
					// Keep the cell alive
					std::cout << "ALIVE" << std::endl;
					rel(*this, up + down + left + right <= 3);
					rel(*this, up + down + left + right >= 2);
				} else {
					std::cout << "DEAD" << std::endl;
					rel(*this, up + down + left + right < 2);
				}
			}
		}

		branch(*this, gridVector, INT_VAR_MIN_MAX(), INT_VAL_MIN());
	}

	// Constructor for cloning
	Life(bool share, Life& life, int size) :
			Script(share, life), n(size) {
		//x.update(*this, share, square.x);
		gridVector.update(*this, share, life.gridVector);
	}

	/// Perform copying during cloning
	virtual Space*
	copy(bool share) {
		return new Life(share, *this, n);
	}

	virtual void print(std::ostream& os) const {
		for (int i = 1; i <= gridVector.size(); i++) {
			std::cout << gridVector[i - 1] << " ";
			if ((i % (n + 4)) == 0) {
				std::cout << std::endl;
			}
		}
	}
};

int main(int argc, char* argv[]) {
	SizeOptions opt("Game of Life");
	opt.size(7);
	opt.parse(argc, argv);
	Script::run<Life, BAB, SizeOptions>(opt);
	return 0;
}
