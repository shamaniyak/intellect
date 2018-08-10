import QtQuick 2.10

Diff {
	id: textDiff
	property int posStart: -1
	property int posEnd: -1
	property int lenSum: 0
	property int lenSumAdd: 0
	property bool canPrintLog: false

	function diff2Texts(_a, _b) {
		diff(_a, _b)

		if(canPrintLog)
			printLog()
	}

	function printLog() {
		console.log("editdistance:" + editdistance());
		//console.log("lcs:" + getlcs());
		console.log("ses");

		var SES = ["-", "", "+"]
		var i   = 0;
		var ses = getses();
		var ses_t = ses.length ? ses[0].t : ses_COMMON
		var ses_elem = ""
		for (i=0;i<ses.length;++i) {
			if(ses[i].t === ses_t)
				ses_elem += ses[i].elem
			else {
				console.log(SES[ses_t+1], ses_elem)
				ses_t = ses[i].t
				ses_elem = ses[i].elem
			}
		}
		console.log(SES[ses_t+1], ses_elem)
	}

	function findFirst() {
		posStart = -1
		lenSum = 0
		lenSumAdd = 0
		return findPos()
	}

	function findPos() {
		console.debug(posStart)
		var ses = getses();
		var i   = posStart < 0 ? 0 : posStart;
		var len = 0;
		var ses_t = i < ses.length ? ses[i].t : ses_COMMON
		for (;i<ses.length;++i) {
			if(ses[i].t === ses_t) {
				len++
				continue
			}
			else {
				posStart = i
				if(ses_t === ses_ADD || ses_t === ses_DELETE) {
					return makePos(ses_t, i, len)
				}

				ses_t = ses[i].t
				len = 1
			}
		}
		var result = {"pos": 0, "len": 0, "t": 0}
		if(ses_t === ses_ADD || ses_t === ses_DELETE) {
			result = makePos(ses_t, i, len)
		}
		posStart = -1
		lenSum = 0
		lenSumAdd = 0
		return result
	}

	function makePos(t, p, l) {
		if(t === ses_ADD) {
			lenSumAdd += l
			return {"pos": p-l-lenSum, "len": l, "t": t}
		}
		if(t === ses_DELETE) {
			lenSum += l
			return {"pos": p-l-lenSumAdd, "len": l, "t": t}
		}
		return {"pos": 0, "len": 0, "t": 0}
	}
}
