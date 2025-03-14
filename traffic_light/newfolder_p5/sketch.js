function setup() {
  let canvas = createCanvas(windowWidth, windowHeight);
  background(220);
}

function draw() {
  r=random(255);
  g=random(255);
  b=random(255);
  fill(r,g,b,100);
  s= random(100)+50;
  circle(mouseX, mouseY, 20);
}
function keyPressed() {
  if(key==='F'||key==='f'){
    let fs=fullscreen();
    fullscreen(!fs);
  }
}
// let handPose;
// let video;
// let hands = [];

// function preload() {
//   handPose = ml5.handPose();
// }

// function setup() {
//   createCanvas(640, 480);
//   video = createCapture(VIDEO, {flipped:true});
//   video.size(640, 480);
//   video.hide();
//   handPose.detectStart(video, gotHands);
// }

// function draw() {
//   image(video, 0, 0, width, height);
//   for (let i = 0; i < hands.length; i++) {
//     let hand = hands[i];
//     for (let j = 0; j < hand.keypoints.length; j++) {
//       let keypoint = hand.keypoints[j];
//       fill(255, 0, 0);
//       noStroke();
//       circle(640-keypoint.x, keypoint.y, 10);
//     }
//   }
// }

// function gotHands(results) {
//   hands = results;
// }
