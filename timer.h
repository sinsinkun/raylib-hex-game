class BurstTimer {
  public:
    float duration;
    bool repeat = false;
    void update(float d);
    bool tick();
    void reset();
  private:
    float dt = 0;
    bool timeUp = false;
};