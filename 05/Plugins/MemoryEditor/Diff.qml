import QtQuick 2.10

/**
 * The algorithm implemented here is based on "An O(NP) Sequence Comparison Algorithm"
 * by described by Sun Wu, Udi Manber and Gene Myers
*/

Item {
	id: root

	property int ses_DELETE: -1
	property int ses_COMMON: 0
	property int ses_ADD: 1

	QtObject {
		id: d
		property string a
		property string b
		property int m
		property int n
		property bool reverse: false
		property var ed: null
		property int offset
		property var path: []
		property var pathposi: []
		property var ses: []
		property string lcs: ""
		property var tmp1
		property var tmp2

		function init() {
			m = a.length
			n = b.length
			reverse = false
			ed = null
			offset = m + 1
			path = []
			pathposi = []
			ses = []
			lcs = ""
			if (m >= n) {
				tmp1    = a
				tmp2    = m
				a       = b
				b       = tmp1
				m       = n
				n       = tmp2
				reverse = true
				offset  = m + 1
			}
		}

		function _P(x, y, k) {
			return {
				'x' : x,
				'y' : y,
				'k' : k,
			}
		}

		function seselem(elem, t) {
			return {
				'elem' : elem,
				't'    : t,
			}
		}

		function snake(k, p, pp) {
			var r, x, y;
			if (p > pp) {
				r = path[k-1+offset];
			} else {
				r = path[k+1+offset];
			}

			y = Math.max(p, pp);
			x = y - k;
			while (x < m && y < n && a[x] === b[y]) {
				++x;
				++y;
			}

			path[k+offset] = pathposi.length;
			pathposi[pathposi.length] = _P(x, y, r);
			return y;
		}

		function recordseq(epc) {
			var x_idx, y_idx, px_idx, py_idx, i;
			x_idx  = y_idx  = 1;
			px_idx = py_idx = 0;
			for (i=epc.length-1;i>=0;--i) {
				while(px_idx < epc[i].x || py_idx < epc[i].y) {
					if (epc[i].y - epc[i].x > py_idx - px_idx) {
						if (reverse) {
							ses[ses.length] = seselem(b[py_idx], ses_DELETE);
						} else {
							ses[ses.length] = seselem(b[py_idx], ses_ADD);
						}
						++y_idx;
						++py_idx;
					} else if (epc[i].y - epc[i].x < py_idx - px_idx) {
						if (reverse) {
							ses[ses.length] = seselem(a[px_idx], ses_ADD);
						} else {
							ses[ses.length] = seselem(a[px_idx], ses_DELETE);
						}
						++x_idx;
						++px_idx;
					} else {
						ses[ses.length] = seselem(a[px_idx], ses_COMMON);
						lcs += a[px_idx];
						++x_idx;
						++y_idx;
						++px_idx;
						++py_idx;
					}
				}
			}
		}

		function compose() {
			var delta, size, fp, p, r, epc, i, k;
			delta  = n - m;
			size   = m + n + 3;
			fp     = {};
			for (i=0;i<size;++i) {
				fp[i] = -1;
				path[i] = -1;
			}
			p = -1;
			do {
				++p;
				for (k=-p;k<=delta-1;++k) {
					fp[k+offset] = snake(k, fp[k-1+offset]+1, fp[k+1+offset]);
				}
				for (k=delta+p;k>=delta+1;--k) {
					fp[k+offset] = snake(k, fp[k-1+offset]+1, fp[k+1+offset]);
				}
				fp[delta+offset] = snake(delta, fp[delta-1+offset]+1, fp[delta+1+offset]);
			} while (fp[delta+offset] !== n);

			ed = delta + 2 * p;

			r = path[delta+offset];

			epc  = [];
			while (r !== -1) {
				epc[epc.length] = _P(pathposi[r].x, pathposi[r].y, null);
				r = pathposi[r].k;
			}
			recordseq(epc);
		}
	}

	function diff(a_, b_) {
		d.a = a_
		d.b = b_

		d.init();

		d.compose()
	}


	function editdistance() {
		return d.ed
	}
	function getlcs() {
		return d.lcs
	}
	function getses() {
		return d.ses
	}
}
