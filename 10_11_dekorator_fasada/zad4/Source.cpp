#include<iostream>
#include<vector>
#include<unordered_map>

enum class Resolution {Low, Normal, High, Auto};

class Subtitle {

};

class StreamingSources {
public:
	static std::vector<std::string> getStreamingSources(std::string movieName) {
		std::vector<std::string> sources;
		sources.push_back("https:/source1/" + movieName);
		sources.push_back("https:/source2/" + movieName);
		return sources;
	}
};

class MovieToHash {
public:
	static int toHash(std::string movieName) { return 123; }
};

class SubtitleDownloader {
public:
	static std::vector<Subtitle*> getSubtitles(int hashCode, std::string language) {
		std::vector<Subtitle*> v;
		v.push_back(new Subtitle());
		v.push_back(new Subtitle());
		return v;
	}
};

class NetworkSpeedMeter {
public:
	static int getDownloadSpeed() { return 100; }
	static int getUploadSpeed() { return 20; }
};

class MovieStream {
public:
	static void playVideo(std::string streamingSource, Resolution resoulution, std::vector<Subtitle*> subtitles,
		int selectedSubtitle = -1) {
		std::cout << "Started streming from url: " << streamingSource << std::endl;
	}

	static Resolution assessOptimalResolution() {
		int speed = NetworkSpeedMeter::getDownloadSpeed();
		if (speed >= 100) return Resolution::High;
		if (speed >= 50) return Resolution::Normal;
		return Resolution::Low;
	}
};

class MovieStreamFacade {
public:
	static void playMovie(std::string movieName) {
		std::vector<std::string> streamingSources = StreamingSources::getStreamingSources(movieName);
		int hash = MovieToHash::toHash(movieName);
		std::vector<Subtitle*> s_serbian = SubtitleDownloader::getSubtitles(hash, "serbian");
		std::vector<Subtitle*> s_eng = SubtitleDownloader::getSubtitles(hash, "english");
		std::vector<Subtitle*> subtitles;
		subtitles.push_back(s_serbian[0]);
		subtitles.push_back(s_eng[0]);
		int selected_subtitle = 0;
		std::string streamingSource = streamingSources[0];
		Resolution r = MovieStream::assessOptimalResolution();
		MovieStream::playVideo(streamingSource, r, subtitles, selected_subtitle);
	}
};

int main() {
	MovieStreamFacade::playMovie("Ko to tamo peva");
	return 0;
}