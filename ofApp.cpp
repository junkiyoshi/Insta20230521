#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	auto seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

	this->face.clear();
	this->line.clear();

	float threshold_1 = 0.47;
	float threshold_2 = 0.53;
	float deg_span = 0.3;
	float z_span = 1;

	float noise_span = 0.005;
	float noise_seed = ofRandom(1000);

	ofColor face_color(255);
	ofColor line_color(0);

	for (float radius = 320; radius <= 340; radius += 2) {

		noise_seed += 0.05;

		for (float deg = 0; deg < 360; deg += deg_span) {

			for (float z = -300; z < 4000; z += z_span) {

				auto noise_value = ofNoise(glm::vec4(radius * cos(deg * DEG_TO_RAD) * noise_span, radius * sin(deg * DEG_TO_RAD) * noise_span, z * noise_span, noise_seed - ofGetFrameNum() * 0.05));
				if (noise_value <= threshold_1 || noise_value >= threshold_2) { continue; }

				auto noise_1 = ofNoise(glm::vec4(radius * cos((deg - deg_span) * DEG_TO_RAD) * noise_span, radius * sin((deg - deg_span) * DEG_TO_RAD) * noise_span, z * noise_span, noise_seed - ofGetFrameNum() * 0.05));
				auto noise_2 = ofNoise(glm::vec4(radius * cos(deg * DEG_TO_RAD) * noise_span, radius * sin(deg * DEG_TO_RAD) * noise_span, (z + z_span) * noise_span, noise_seed - ofGetFrameNum() * 0.05));
				auto noise_3 = ofNoise(glm::vec4(radius * cos(deg * DEG_TO_RAD) * noise_span, radius * sin(deg * DEG_TO_RAD) * noise_span, (z - z_span) * noise_span, noise_seed - ofGetFrameNum() * 0.05));
				auto noise_4 = ofNoise(glm::vec4(radius * cos((deg + deg_span) * DEG_TO_RAD) * noise_span, radius * sin((deg + deg_span) * DEG_TO_RAD) * noise_span, z * noise_span, noise_seed - ofGetFrameNum() * 0.05));

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec3(radius * cos((deg - deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg - deg_span * 0.5) * DEG_TO_RAD), z - z_span * 0.5));
				vertices.push_back(glm::vec3(radius * cos((deg + deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg + deg_span * 0.5) * DEG_TO_RAD), z - z_span * 0.5));
				vertices.push_back(glm::vec3(radius * cos((deg - deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg - deg_span * 0.5) * DEG_TO_RAD), z + z_span * 0.5));
				vertices.push_back(glm::vec3(radius * cos((deg + deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg + deg_span * 0.5) * DEG_TO_RAD), z + z_span * 0.5));

				this->face.addVertices(vertices);

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
				this->face.addIndex(index + 0); this->face.addIndex(index + 2); this->face.addIndex(index + 3);

				if (noise_1 <= threshold_1 || noise_1 >= threshold_2) {

					this->line.addVertex(vertices[0]);
					this->line.addVertex(vertices[2]);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);
				}

				if (noise_2 <= threshold_1 || noise_2 >= threshold_2) {

					this->line.addVertex(vertices[2]);
					this->line.addVertex(vertices[3]);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);
				}

				if (noise_3 <= threshold_1 || noise_3 >= threshold_2) {

					this->line.addVertex(vertices[0]);
					this->line.addVertex(vertices[1]);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);
				}

				if (noise_4 <= threshold_1 || noise_4 >= threshold_2) {

					this->line.addVertex(vertices[1]);
					this->line.addVertex(vertices[3]);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(180);

	ofSetColor(0);
	this->line.draw();

	ofSetColor(255);
	this->face.draw();

	/*
	ostringstream os;
	os << setw(4) << setfill('0') << ofGetFrameNum() + 1;
	ofImage image;
	image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	image.saveImage("image/img_" + os.str() + ".jpg");
	if (ofGetFrameNum() + 1 >= 24 * 20) {

		std::exit(1);
	}
	*/

	// > ffmpeg -i img_%04d.jpg -r 24 out.mp4


	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}