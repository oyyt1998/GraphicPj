	var AudioContext,context,source,analyser,p,penBg,dataArray,gradient,gradientRight;

	var screenHeight = document.body.clientHeight,
	screenWidth = document.body.clientWidth;

	//Canvas初始化
	var width = canvas.width,
	height = canvas.height;
	var name = "短裙-AOA.flac";
	//选定播放曲目
	/*var t_files = this.files();
	for(var i=0, len= t_files.length; i<len; i++){
		if(t_files[i].match('.flac')||t_files[i].match('.mp3')||t_files[i].match('.mp4')){
			name = t_files[i];
			break;
		}
	};*/
	var audio = new Audio(name);
	audio.charset = 'utf-8';

	audio.oncanplaythrough = function() {
		//if(screenWidth!=width || screenHeight!=height){
		//	zoomPage();
		loader.innerHTML = '<div>点我</div>';
		//}

		document.addEventListener('click',function(){
			init();
			loader.style.display = 'none'
			song_name.style.display = 'block'
			var sn = document.getElementById("song_name");
			sn.innerHTML = '<div id="song_name">'+ name.split('.')[0] + '</div>';
			//播放曲目
			audio.play();
			//频谱动画开始
			draw();
			//辐射层透明，这样才能显示后面的频谱
			penBg.globalAlpha = 0.2;
			document.removeEventListener('click',arguments.callee);
		})
	};

	function init(){

		AudioContext = AudioContext || webkitAudioContext;
		context = new AudioContext;
		source = context.createMediaElementSource(audio);
		analyser = context.createAnalyser();
		source.connect(analyser);
		analyser.connect(context.destination);

		p = canvas.getContext("2d");
		penBg = bg.getContext("2d");

		analyser.fftSize = 4096;
		var length = analyser.fftSize;
		
		//创建数据
		dataArray = new Uint8Array(length);

		//填充渐变色
		gradient = p.createLinearGradient(0, 100, 480, 100);
		gradient.addColorStop("0", "#ff0055");
		gradient.addColorStop("1.0", "#ceaf11");
}

function draw() {

		requestAnimationFrame(draw)

		analyser.getByteFrequencyData(dataArray);

		p.clearRect(0, 0, width, height);

		//制作放射性背景，设定为一个圆圈
		var gradientBg = penBg.createRadialGradient(width / 2, height / 2, height - Math.floor(Math.random() * 150 + 100), width / 2, height / 2, width / 2);
		gradientBg.addColorStop(0, "white");
		gradientBg.addColorStop(1, '#000');

		penBg.clearRect(0, 0, width, height);
		penBg.fillStyle = gradientBg;
		penBg.fillRect(0, 0, width, height);

		//频谱密集程度
	    var m = 5;

	    //中轴线位置
	    var n = height / 3;

		//填充部分
		p.beginPath();
		p.moveTo(0, height - n);
		var x = 0;
		for (var i = 1; i < width; i++) {
			var lineHeight = dataArray[i] / 256 * height / 3;
			if (i < m) {
				p.lineTo(x, height - dataArray[i] / 256 * height / 2 - n)
			} else if (i > width - m) {
				p.lineTo(x - m, height - n)
			} else {
				p.lineTo(x, height - lineHeight - n)
			}
			x += m - 1;
		}
		p.fillStyle = gradient;
		p.fill();
		p.closePath();

		//线条部分
		p.beginPath();
		p.moveTo(0, height - n);
		var x = 0;

		for (var i = 1; i < width; i++) {
			var lineHeight = dataArray[i] / 256 * height / 3;
			if (i < m) {
				p.lineTo(x, height - dataArray[i] / 256 * height / 2 - n - 10 - Math.floor(Math.random() * 30))
			} else if (i > width - m) {
				p.lineTo(x - m, height - n - 20)
			} else {
				p.lineTo(x, height - lineHeight - n - 10 - Math.floor(Math.random() * 30))
			}
			x += m - 1;
		}
		p.strokeStyle = gradient;
		p.stroke();
		p.closePath();

		//倒影
		p.beginPath();
		p.moveTo(0, height - n);
		var x = 0;
		for (var i = 1; i < width; i++) {
			var lineHeight = dataArray[i] / 256 * height / 40;
			if (i < m) {
				p.lineTo(x, dataArray[i] / 256 * height / 24 + height - n)
			} else p.lineTo(x, lineHeight + height - n)
			x += m - 1;
		}
		p.lineTo(x - m, height - n)
		p.fillStyle = '#21dd13';

		p.shadowBlur = 20;
		p.shadowColor = "#21dd13";
		p.fill();
		p.closePath();
		p.shadowBlur = 0;

	}