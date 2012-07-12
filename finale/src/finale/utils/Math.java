package finale.utils;

/**
 * 
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public class Math {

        public static final double PRECISION = 1e-12;

	private static double[] pow10 = {1.0, //
		10.0, //
		100.0, //
		1000.0, //
		10000.0, //
		100000.0, //
		1000000.0, //
		10000000.0, //
		100000000.0, //
		1000000000.0, //
		10000000000.0};

	public static double Round(double number, int nDecimals) {

		boolean positive = true;
		if (number < 0.0) {
			positive = false;
			number = -number;
		}

		double pow;
		if (nDecimals >= 0 && nDecimals <= 10) {
			pow = pow10[nDecimals];
		} else {
			pow = java.lang.Math.pow(10.0, nDecimals);
		}

		double result = java.lang.Math.round(number * pow) / pow;

		if (positive) {
			return result;
		} else {
			return -result;
		}

	}

	/**
	 * Function RoundDown always "rounds down".
	 * @param number Number, that should be rounded down.
	 * @param nDecimals Number of digits to which the number should be rounded down.
	 * @return Function RoundDown rounds down, that is "forgets" insignificant
	 * digits. <ul>
	 * <li>RoundDown(2.78,0) returns 2.0</li>
	 * <li>RoundDown(3.1415279,2) returns 3.14</li>
	 * <li>RoundDown(-1.5,0) returns -1.0</li>
	 * <li>RoundDown(1234,-3) returns 1000.0</li>
	 * </ul>
	 */
	public static double RoundDown(double number, int nDecimals) {

		boolean positive = true;
		if (number < 0.0) {
			positive = false;
			number = -number;
		}

		double pow;
		if (nDecimals >= 0 && nDecimals <= 10) {
			pow = pow10[nDecimals];
		} else {
			pow = java.lang.Math.pow(10.0, nDecimals);
		}

		double result = java.lang.Math.floor(number * pow) / pow;

		if (positive) {
			return result;
		} else {
			return -result;
		}

	}

	/**
	 * Function RoundUp always "rounds up".
	 * @param number Number, that should be rounded up.
	 * @param nDecimals Number of digits to which the number should be rounded down.
	 * @return Function RoundDown rounds up, that is returns the smallest double
	 * value greater than the argument number that is equal to a number rounded
	 * to nDecimal digits. <ul>
	 * <li>RoundUp(2.78,0) returns 3.0</li>
	 * <li>RoundUp(3.1415279,2) returns 4.0</li>
	 * <li>RoundUp(-1.5,0) returns -2.0</li>
	 * <li>RoundUp(1234,-3) returns 2000.0</li>
	 * </ul>
	 */
	public static double RoundUp(double number, int nDecimals) {

		boolean positive = true;
		if (number < 0.0) {
			positive = false;
			number = -number;
		}

		double pow;
		if (nDecimals >= 0 && nDecimals <= 10) {
			pow = pow10[nDecimals];
		} else {
			pow = java.lang.Math.pow(10.0, nDecimals);
		}

		double result = java.lang.Math.ceil(number * pow) / pow;

		if (positive) {
			return result;
		} else {
			return -result;
		}

	}


}
